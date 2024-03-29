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

test_that("Min and max can be equal for scalar numbers", {
  expect_true(seed_uniform_scalar_min_eq_max(seed))
})

test_that("Min must not be larger than max", {
  expect_error(seed_uniform_scalar_min_gt_max(seed),
               "'min' must not be larger than 'max'!")
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
  expect_true(xoroshiro_plus_jump())
})

test_that("jump() for xoroshiro128++ works", {
  expect_true(xoroshiro_plus_plus_jump())
})

test_that("jump() for xoroshiro128** works", {
  expect_true(xoroshiro_star_star_jump())
})

test_that("long_jump() for xoshiro256+ works", {
  expect_true(xoshiro_plus_long_jump())
})

test_that("long_jump() for xoshiro256++ works", {
  expect_true(xoshiro_plus_plus_long_jump())
})

test_that("long_jump() for xoshiro256** works", {
  expect_true(xoshiro_star_star_long_jump())
})
