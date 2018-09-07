context("C++")

Rcpp::sourceCpp("cpp/default.cpp")

seed <- 1234567890

test_that("consecutive calls yield different random numbers", {
  expect_false(consecutive_calls(seed))
})

test_that("setting seed produces identical uniformly distributed numbers", {
  expect_true(seed_uniform(seed))
})

test_that("setting seed produces identical uniformly distributed scalar numbers", {
  expect_true(seed_uniform_scalar(seed))
})

test_that("setting seed produces identical normaly distributed numbers", {
  expect_true(seed_normal(seed))
})

test_that("setting seed produces identical normaly distributed scalar numbers", {
  expect_true(seed_normal_scalar(seed))
})

test_that("setting seed produces identical exponenetially distributed numbers", {
  expect_true(seed_exponential(seed))
})

test_that("setting seed produces identical exponenetially distributed scalar numbers", {
  expect_true(seed_exponential_scalar(seed))
})

Rcpp::sourceCpp("cpp/xoshiro-jump.cpp")

test_that("jump() for xoroshiro128+ works", {
  expect_true(xoroshiro_jump())
})

test_that("jump() for xoshiro256+ works", {
  expect_true(xoshiro_jump())
})
