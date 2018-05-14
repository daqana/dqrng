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

#include <Rcpp.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>
#include <xoshiro.h>
#include <pcg_random.hpp>


namespace {
dqrng::rng64_t rng = dqrng::generator();
}

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export(rng = false)]]
void dqset_seed(const uint32_t seed) {
  uint64_t seed2  = 1664525 * seed + 1013904223;
  uint64_t _seed = seed || (seed2 << 32);
  rng->seed(_seed);
}

//' @name dqrng
//' @rdname dqrng
//' @export
// [[Rcpp::export(rng = false)]]
void dqRNGkind(std::string kind, const std::string& normal_kind = "ignored") {
  for (auto & c: kind)
    c = std::tolower(c);
  uint64_t seed = rng->operator()();
  if (kind == "default") {
    rng =  dqrng::generator(seed);
  } else if (kind == "mersenne-twister") {
    rng =  dqrng::generator<std::mt19937_64>(seed);
  } else if (kind == "xoroshiro128+") {
    rng =  dqrng::generator<dqrng::xoroshiro128plus>(seed);
  } else if (kind == "xoshiro256+") {
    rng =  dqrng::generator<dqrng::xoshiro256plus>(seed);
  } else if (kind == "pcg64") {
    rng =  dqrng::generator<pcg64>(seed);
  } else {
    Rcpp::stop("Unknown random generator kind: %s", kind);
  }
}

//' @rdname dqrng
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrunif(size_t n, double min = 0.0, double max = 1.0) {
  if(max / 2. - min / 2. > (std::numeric_limits<double>::max)() / 2.)
     return 2. * dqrunif(n, min/2., max/2.);

  dqrng::uniform_distribution dist(min, max);
  return dqrng::generate<dqrng::uniform_distribution, Rcpp::NumericVector>(n, rng, dist);
}

//' @rdname dqrng
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
  dqrng::normal_distribution dist(mean, sd);
  return dqrng::generate<dqrng::normal_distribution, Rcpp::NumericVector>(n, rng, dist);
}

//' @rdname dqrng
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
  dqrng::exponential_distribution dist(rate);
  return dqrng::generate<dqrng::exponential_distribution, Rcpp::NumericVector>(n, rng, dist);
}
