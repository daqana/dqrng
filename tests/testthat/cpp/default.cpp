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
bool seed_uniform_scalar(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  double u1 = dqrng::runif();
  dqrng::dqset_seed(seed);
  double u2 = dqrng::runif();
  return u1 == u2;
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
bool seed_normal_scalar(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  double n1 = dqrng::rnorm();
  dqrng::dqset_seed(seed);
  double n2 = dqrng::rnorm();
  return n1 == n2;
}

// [[Rcpp::export]]
bool seed_exponential(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector e1 = dqrng::dqrexp(10);
  dqrng::dqset_seed(seed);
  Rcpp::NumericVector e2 = dqrng::dqrexp(10);
  return Rcpp::is_true(Rcpp::all(e1 == e2));
}

// [[Rcpp::export]]
bool seed_exponential_scalar(Rcpp::IntegerVector seed) {
  dqrng::dqset_seed(seed);
  double u1 = dqrng::rexp();
  dqrng::dqset_seed(seed);
  double u2 = dqrng::rexp();
  return u1 == u2;
}
