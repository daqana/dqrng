#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include <dqrng.h>

// [[Rcpp::export]]
bool consecutive_calls(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector u1 = dqrng::dqrunif(10);
  Rcpp::NumericVector u2 = dqrng::dqrunif(10);
  return Rcpp::is_true(Rcpp::all(u1 == u2));
}

// [[Rcpp::export]]
bool seed_uniform(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector u1 = dqrng::dqrunif(10);
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector u2 = dqrng::dqrunif(10);
  return Rcpp::is_true(Rcpp::all(u1 == u2));
}

// [[Rcpp::export]]
bool seed_normal(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector n1 = dqrng::dqrnorm(10);
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector n2 = dqrng::dqrnorm(10);
  return Rcpp::is_true(Rcpp::all(n1 == n2));
}

// [[Rcpp::export]]
bool seed_exponential(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector e1 = dqrng::dqrexp(10);
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector e2 = dqrng::dqrexp(10);
  return Rcpp::is_true(Rcpp::all(e1 == e2));
}

