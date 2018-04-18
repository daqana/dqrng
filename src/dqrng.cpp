#include <dqrng_generator.h>
#include <dqrng_distribution.h>

namespace {
auto rng = dqrng::generator();
}

// [[Rcpp::interfaces(r, cpp)]]
//' @name dqrng functions
//'

// [[Rcpp::export(rng = false)]]
void dqset_seed(const uint32_t seed) {
  uint64_t seed2  = 1664525 * seed + 1013904223;
  uint64_t _seed = seed || (seed2 << 32);
  rng->seed(_seed);
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
void dqRNGkind(const std::string kind, std::string normal_kind = "ignored") {
  rng = dqrng::generator(kind, rng->operator()());
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrunif(size_t n, double min = 0.0, double max = 1.0) {
  if(max / 2 - min / 2 > (std::numeric_limits<double>::max)() / 2)
     return 2 * dqrunif(n, min/2, max/2);

  dqrng::uniform_distribution dist(min, max);
  return dqrng::generate_from_distribution<dqrng::uniform_distribution>(n, rng, dist);
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrnorm(size_t n, double mean = 0.0, double sd = 1.0) {
  dqrng::normal_distribution dist(mean, sd);
  return dqrng::generate_from_distribution<dqrng::normal_distribution>(n, rng, dist);
}

//' @rdname dqrng-functions
//' @export
// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp(size_t n, double rate = 1.0) {
  dqrng::exponential_distribution dist(rate);
  return dqrng::generate_from_distribution<dqrng::exponential_distribution>(n, rng, dist);
}
