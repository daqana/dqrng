// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
// [[Rcpp::depends(dqrng,BH)]]
#include <dqrng.h>
#include <dqrng_distribution.h>

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp_extrng(const std::size_t n, const double rate = 1.0) {
  auto out = Rcpp::NumericVector(Rcpp::no_init(n));
  dqrng::random_64bit_accessor engine{};
  engine.generate<dqrng::exponential_distribution>(out, rate);
  return out;
}

// [[Rcpp::export]]
bool cloned_calls(int stream) {
  dqrng::random_64bit_accessor engine{};
  auto cloned_engine = engine.clone(stream);

  Rcpp::NumericVector u1(10);
  Rcpp::NumericVector u2(10);

  engine.generate<dqrng::uniform_distribution>(u1);
  cloned_engine->generate<dqrng::uniform_distribution>(u2);

  return Rcpp::is_true(Rcpp::all(u1 == u2));
}
