// Copyright 2018 Ralf Stubner (daqana GmbH)
//
// This file is part of dqrng.
//
// dqrng is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dqrng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with dqrng.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/dynamic_bitset.hpp>
#include <Rcpp.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>
#include <xoshiro.h>
#include <pcg_random.hpp>
#include <threefry.h>
#include <convert_seed.h>
#include <R_randgen.h>
#include <minimal_hash_set.h>

namespace {
dqrng::rng64_t init() {
  Rcpp::RNGScope rngScope;
  Rcpp::IntegerVector seed(2, dqrng::R_random_int);
  return dqrng::generator(dqrng::convert_seed<uint64_t>(seed));
}
dqrng::rng64_t rng = init();
}

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export(rng = false)]]
void dqset_seed(Rcpp::IntegerVector seed, Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
  uint64_t _seed = dqrng::convert_seed<uint64_t>(seed);
  if (stream.isNotNull()) {
      uint64_t _stream = dqrng::convert_seed<uint64_t>(stream.as());
    rng->seed(_seed, _stream);
  } else {
    rng->seed(_seed);
  }
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
void dqRNGkind(std::string kind, const std::string& normal_kind = "ignored") {
  for (auto & c: kind)
    c = std::tolower(c);
  uint64_t seed = rng->operator()();
  if (kind == "default") {
    rng =  dqrng::generator(seed);
  } else if (kind == "xoroshiro128+") {
    rng =  dqrng::generator<dqrng::xoroshiro128plus>(seed);
  } else if (kind == "xoshiro256+") {
    rng =  dqrng::generator<dqrng::xoshiro256plus>(seed);
  } else if (kind == "pcg64") {
    rng =  dqrng::generator<pcg64>(seed);
  } else if (kind == "threefry") {
    rng =  dqrng::generator<sitmo::threefry_20_64>(seed);
  } else {
    Rcpp::stop("Unknown random generator kind: %s", kind);
  }
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrunif(size_t n, double min = 0.0, double max = 1.0) {
  if(max / 2. - min / 2. > (std::numeric_limits<double>::max)() / 2.)
     return 2. * dqrunif(n, min/2., max/2.);

  dqrng::uniform_distribution dist(min, max);
  return dqrng::generate<dqrng::uniform_distribution, Rcpp::NumericVector>(n, rng, dist);
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
  dqrng::normal_distribution dist(mean, sd);
  return dqrng::generate<dqrng::normal_distribution, Rcpp::NumericVector>(n, rng, dist);
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
  dqrng::exponential_distribution dist(rate);
  return dqrng::generate<dqrng::exponential_distribution, Rcpp::NumericVector>(n, rng, dist);
}

// [[Rcpp::export(rng = false)]]
Rcpp::IntegerVector dqsample_int(int m,
                                 int n,
                                 bool replace = false,
                                 Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                 int offset = 0) {
    if (!(m > 0 && n >= 0 && m >= n))
        Rcpp::stop("Argument requirements not fulfilled: m > 0 && n >= 0 && m >= n");
    uint32_t _m(m);
    uint32_t _n(n);
    Rcpp::IntegerVector result(Rcpp::no_init(_n));
    if (replace || n <= 1) {
        std::generate(result.begin(), result.end(),
                      [_m, offset] () {return static_cast<int>(offset + (*rng)(_m));});
    } else {
        if (m < 2 * n) {
            Rcpp::IntegerVector tmp(Rcpp::no_init(_m));
            std::iota(tmp.begin(), tmp.end(), offset);

            for (uint32_t i = 0; i < _n; ++i) {
                uint32_t j = i + (*rng)(_m - i);
                std::swap(tmp[i], tmp[j]);
            }
            std::copy(tmp.begin(), tmp.begin() + n, result.begin());
        } else if (m < 1000 * n) {
            boost::dynamic_bitset<> elems(_m);
            for (uint32_t i = 0; i < _n; ++i) {
                for (;;) {
                    uint32_t v = (*rng)(_m);
                    if (!elems.test_set(v)) {
                        result(i) = offset + v;
                        break;
                    }
                }
            }
        } else {
            dqrng::minimal_hash_set<uint32_t> elems(_n);
            for (uint32_t i = 0; i < _n; ++i) {
                for (;;) {
                    uint32_t v = (*rng)(_m);
                    if (elems.insert(v)) {
                        result(i) = offset + v;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqsample_num(double m,
                                 double n,
                                 bool replace = false,
                                 Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                 int offset = 0) {
#ifndef LONG_VECTOR_SUPPORT
    Rcpp::stop("Long vectors are not supported");
#else
  if (!(m > 0 && n >= 0 && m >= n))
    Rcpp::stop("Argument requirements not fulfilled: m > 0 && n >= 0 && m >= n");
  uint64_t _m(m);
  uint64_t _n(n);
  Rcpp::NumericVector result(Rcpp::no_init(_n));
  if (replace || n <= 1) {
    std::generate(result.begin(), result.end(),
                  [_m, offset] () {return static_cast<double>(offset + (*rng)(_m));});
  } else {
    if (m < 2.0 * n) {
      Rcpp::NumericVector tmp(Rcpp::no_init(_m));
      std::iota(tmp.begin(), tmp.end(), offset);

      for (uint64_t i = 0; i < _n; ++i) {
        uint64_t j = i + (*rng)(_m - i);
        std::swap(tmp[i], tmp[j]);
      }
      std::copy(tmp.begin(), tmp.begin() + _n, result.begin());
    } else if (m < 1000.0 * n) {
      boost::dynamic_bitset<> elems(_m);
      for (uint64_t i = 0; i < _n; ++i) {
        for (;;) {
          uint64_t v = (*rng)(_m);
          if (!elems.test_set(v)) {
            result(i) = offset + v;
            break;
          }
        }
      }
    } else {
      dqrng::minimal_hash_set<uint64_t> elems(_n);
      for (uint64_t i = 0; i < n; ++i) {
        for (;;) {
          uint64_t v = (*rng)(_m);
          if (elems.insert(v)) {
            result(i) = offset + v;
            break;
          }
        }
      }
    }
  }
  return result;
#endif
}
