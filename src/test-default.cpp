#include <Rcpp.h>
#include <testthat.h>
#include <dqrng.h>

int seed = 1234567890;

context("default generator") {
  test_that("consecutive calls yield different random numbers") {
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector u1 = dqrng::dqrunif(10);
    Rcpp::NumericVector u2 = dqrng::dqrunif(10);
    expect_false(Rcpp::is_true(Rcpp::all(u1 == u2)));
  };

  test_that("setting seed produces identical uniformly distributed numbers") {
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector u1 = dqrng::dqrunif(10);
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector u2 = dqrng::dqrunif(10);
    expect_true(Rcpp::is_true(Rcpp::all(u1 == u2)));
  }

  test_that("setting seed produces identical normally distributed numbers") {
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector n1 = dqrng::dqrnorm(10);
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector n2 = dqrng::dqrnorm(10);
    expect_true(Rcpp::is_true(Rcpp::all(n1 == n2)));
  }

  test_that("setting seed produces identical exponentially distributed numbers") {
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector e1 = dqrng::dqrexp(10);
    dqrng::dqset_seed(seed);
    Rcpp::NumericVector e2 = dqrng::dqrexp(10);
    expect_true(Rcpp::is_true(Rcpp::all(e1 == e2)));
  }
}
