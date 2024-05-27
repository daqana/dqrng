// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2022-2024 Ralf Stubner
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
#include <dqrng_threefry.h>

namespace {
dqrng::rng64_t rng = dqrng::generator(56478348);
std::string rng_kind = "default";
}

// [[Rcpp::interfaces(r, cpp)]]

// [[Rcpp::export(rng = false)]]
void dqset_seed(Rcpp::Nullable<Rcpp::IntegerVector> seed,
                Rcpp::Nullable<Rcpp::IntegerVector> stream = R_NilValue) {
  if (seed.isNull()) {
    rng = dqrng::generator();
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
  rng_kind = kind;
  if (kind == "default") {
    rng =  dqrng::generator(seed);
  } else if (kind == "xoroshiro128+") {
    rng =  dqrng::generator<dqrng::xoroshiro128plus>(seed);
  } else if (kind == "xoroshiro128++") {
    rng =  dqrng::generator<dqrng::xoroshiro128plusplus>(seed);
  } else if (kind == "xoshiro256+") {
    rng =  dqrng::generator<dqrng::xoshiro256plus>(seed);
  } else if (kind == "xoshiro256++") {
    rng =  dqrng::generator<dqrng::xoshiro256plusplus>(seed);
#if !(defined(__APPLE__) && defined(__POWERPC__))
  } else if (kind == "pcg64") {
    rng =  dqrng::generator<pcg64>(seed);
#endif
  } else if (kind == "threefry") {
    rng =  dqrng::generator<sitmo::threefry_20_64>(seed);
  } else {
    Rcpp::stop("Unknown random generator kind: %s", kind);
  }
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
std::vector<std::string> dqrng_get_state() {
  std::stringstream buffer;
  buffer << rng_kind << " " << *rng;
  std::vector<std::string> state{std::istream_iterator<std::string>{buffer},
                                 std::istream_iterator<std::string>{}};
  return state;
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
void dqrng_set_state(std::vector<std::string> state) {
  std::stringstream buffer;
  std::copy(state.begin() + 1,
            state.end(),
            std::ostream_iterator<std::string>(buffer, " "));
  dqRNGkind(state[0]);
  buffer >> *rng;
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

  auto out = Rcpp::NumericVector(Rcpp::no_init(n));
  rng->generate<dqrng::uniform_distribution>(out, min, max);
  return out;
}

// [[Rcpp::export(rng = false)]]
double runif(double min = 0.0, double max = 1.0) {
  if (min > max)
    Rcpp::stop("'min' must not be larger than 'max'!");
  if (min == max)
    return min;
  if (max / 2. - min / 2. > (std::numeric_limits<double>::max)() / 2.)
    return 2. * runif(min/2., max/2.);

  return rng->variate<dqrng::uniform_distribution>(min, max);;
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
  auto out = Rcpp::NumericVector(Rcpp::no_init(n));
  rng->generate<dqrng::normal_distribution>(out, mean, sd);
  return out;
}

// [[Rcpp::export(rng = false)]]
double rnorm(double mean = 0.0, double sd = 1.0) {
  return rng->variate<dqrng::normal_distribution>(mean, sd);;
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
  auto out = Rcpp::NumericVector(Rcpp::no_init(n));
  rng->generate<dqrng::exponential_distribution>(out, rate);
  return out;
}

// [[Rcpp::export(rng = false)]]
double rexp(double rate = 1.0) {
  return rng->variate<dqrng::exponential_distribution>(rate);;
}

//' @keywords internal
// [[Rcpp::export(rng = false)]]
Rcpp::XPtr<dqrng::random_64bit_generator> get_rng() {
  return Rcpp::XPtr<dqrng::random_64bit_generator>(rng);
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
Rcpp::IntegerVector dqsample_int(int n,
                                 int size,
                                 bool replace = false,
                                 Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                 int offset = 0) {
    if (!(n > 0 && size >= 0))
      Rcpp::stop("Argument requirements not fulfilled: n > 0 && size >= 0");
    return dqrng::sample::sample<Rcpp::IntegerVector, uint32_t>(*rng, uint32_t(n), uint32_t(size), replace, offset);
}

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqsample_num(double n,
                                 double size,
                                 bool replace = false,
                                 Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                 int offset = 0) {
    if (!(n > 0 && size >= 0))
      Rcpp::stop("Argument requirements not fulfilled: n > 0 && size >= 0");
#ifndef LONG_VECTOR_SUPPORT
    Rcpp::stop("Long vectors are not supported");
#else
    return dqrng::sample::sample<Rcpp::NumericVector, uint64_t>(*rng, uint64_t(n), uint64_t(size), replace, offset);
#endif
}

extern "C" {
// allow registering as user-supplied RNG
double * user_unif_rand(void) {
  static double res;
  res = rng->uniform01();
  return &res;
}

// https://stackoverflow.com/a/47839021/8416610
Int32 unscramble(Int32 u) {
  for (int j = 0; j < 50; ++j) {
    u = ((u - 1) * 2783094533UL);
  }
  return u;
}

void user_unif_init(Int32 seed_in) {
  rng->seed(uint64_t(unscramble(seed_in)));
}

double * user_norm_rand(void) {
  static double res;
  res = rng->variate<dqrng::normal_distribution>(0.0, 1.0);
  return &res;
}
} // extern "C"

static const R_CMethodDef cMethods[] = {
  {"user_unif_rand", (DL_FUNC) &user_unif_rand, 0, NULL},
  {"user_unif_init", (DL_FUNC) &user_unif_init, 0, NULL},
  {"user_norm_rand", (DL_FUNC) &user_norm_rand, 0, NULL},
  {NULL, NULL, 0, NULL}
};

// [[Rcpp::init]]
void dqrng_init(DllInfo *dll) {
  R_registerRoutines(dll, cMethods, NULL, NULL, NULL);
}
