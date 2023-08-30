// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2022 Ralf Stubner
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

#include <mystdint.h>
#include <Rcpp.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>
#include <dqrng_sample.h>
#include <xoshiro.h>
#include <pcg_random.hpp>
#include <threefry.h>
#include <convert_seed.h>
#include <R_randgen.h>

namespace {
dqrng::rng64_t init() {
  Rcpp::RNGScope rngScope;
  Rcpp::IntegerVector seed(2, dqrng::R_random_int);
  return dqrng::generator(dqrng::convert_seed<uint64_t>(seed));
}
dqrng::rng64_t rng = nullptr;

using generator = double(*)();
dqrng::uniform_distribution uniform{};
generator runif_impl = [] () {return uniform(*rng);};
dqrng::normal_distribution normal{};
generator rnorm_impl = [] () {return normal(*rng);};
dqrng::exponential_distribution exponential{};
generator rexp_impl = [] () {return exponential(*rng);};
}

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export(rng = false)]]
void dqset_seed(Rcpp::Nullable<Rcpp::IntegerVector> seed,
                Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
  if (seed.isNull()) {
    rng = init();
  } else {
    uint64_t _seed = dqrng::convert_seed<uint64_t>(seed.as());
    if (stream.isNotNull()) {
      uint64_t _stream = dqrng::convert_seed<uint64_t>(stream.as());
      rng->seed(_seed, _stream);
    } else {
      rng->seed(_seed);
    }
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
  if (min > max)
    Rcpp::stop("Error: 'min' must not be larger than 'max'!");
  if (min == max)
    return Rcpp::NumericVector(n, min);
  if(max / 2. - min / 2. > (std::numeric_limits<double>::max)() / 2.)
    return 2. * dqrunif(n, min/2., max/2.);

  using parm_t = decltype(uniform)::param_type;
  uniform.param(parm_t(min, max));
  return Rcpp::NumericVector(n, runif_impl);
}

// [[Rcpp::export(rng = false)]]
double runif(double min = 0.0, double max = 1.0) {
  if (min > max)
    Rcpp::stop("'min' must not be larger than 'max'!");
  if (min == max)
    return min;
  if (max / 2. - min / 2. > (std::numeric_limits<double>::max)() / 2.)
    return 2. * runif(min/2., max/2.);

  using parm_t = decltype(uniform)::param_type;
  uniform.param(parm_t(min, max));
  return runif_impl();
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
  using parm_t = decltype(normal)::param_type;
  normal.param(parm_t(mean, sd));
  return Rcpp::NumericVector(n, rnorm_impl);
}

// [[Rcpp::export(rng = false)]]
double rnorm(double mean = 0.0, double sd = 1.0) {
  using parm_t = decltype(normal)::param_type;
  normal.param(parm_t(mean, sd));
  return rnorm_impl();
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
  using parm_t = decltype(exponential)::param_type;
  exponential.param(parm_t(rate));
  return Rcpp::NumericVector(n, rexp_impl);
}

// [[Rcpp::export(rng = false)]]
double rexp(double rate = 1.0) {
  using parm_t = decltype(exponential)::param_type;
  exponential.param(parm_t(rate));
  return rexp_impl();
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::IntegerVector dqrrademacher(size_t n) {
  Rcpp::IntegerVector res = Rcpp::no_init(n);
  size_t k = 0;
  for (size_t i = 0; i < ceil(n / 64.0) - 1; ++i) {
    uint64_t bits = (*rng)();

    for (int j = 0; j <= 63; ++j, ++k) {
      res[k] = ((bits >> j) & 1) * 2 - 1;
    }
  }

  uint64_t bits = (*rng)();
  for (int j = 0; k < n; ++j, ++k) {
    res[k] = ((bits >> j) & 1) * 2 - 1;
  }

  return res;
}

// [[Rcpp::export(rng = false)]]
Rcpp::IntegerVector dqsample_int(int m,
                                 int n,
                                 bool replace = false,
                                 Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                 int offset = 0) {
    if (!(m > 0 && n >= 0))
        Rcpp::stop("Argument requirements not fulfilled: m > 0 && n >= 0");
    return dqrng::sample::sample<INTSXP, uint32_t>(rng, uint32_t(m), uint32_t(n), replace, offset);
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
    if (!(m > 0 && n >= 0))
        Rcpp::stop("Argument requirements not fulfilled: m > 0 && n >= 0");
    return dqrng::sample::sample<REALSXP, uint64_t>(rng, uint64_t(m), uint64_t(n), replace, offset);
#endif
}
