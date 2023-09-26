context("default generator")

seed <- 1234567890

test_that("consecutive calls yield different random numbers", {
  dqset.seed(seed)
  u1 <- dqrunif(10)
  u2 <- dqrunif(10)
  expect_false(all(u1 == u2))
})

test_that("setting seed produces identical uniformly distributed numbers", {
  dqset.seed(seed)
  u1 <- dqrunif(10)
  dqset.seed(seed)
  u2 <- dqrunif(10)
  expect_equal(u1, u2)
})

test_that("setting seed produces identical uniformly distributed numbers (user defined RNG)", {
  orig <- RNGkind("user", "default")
  dqset.seed(seed)
  u1 <- dqrunif(10)
  set.seed(seed)
  u2 <- stats::runif(10)
  expect_equal(u1, u2)
  RNGkind(orig[1], orig[2], orig[3])
})

test_that("setting seed produces identical normaly distributed numbers", {
  dqset.seed(seed)
  n1 <- dqrnorm(10)
  dqset.seed(seed)
  n2 <- dqrnorm(10)
  expect_equal(n1, n2)
})

test_that("setting seed produces identical normaly distributed numbers (user defined RNG)", {
  orig <- RNGkind("user", "user")
  dqset.seed(seed)
  n1 <- dqrnorm(10)
  set.seed(seed)
  n2 <- stats::rnorm(10)
  expect_equal(n1, n2)
  RNGkind(orig[1], orig[2], orig[3])
})

test_that("setting seed produces identical exponenetially distributed numbers", {
  dqset.seed(seed)
  n1 <- dqrexp(10)
  dqset.seed(seed)
  n2 <- dqrexp(10)
  expect_equal(n1, n2)
})

test_that("numbers are generated in the correct range", {
  dqset.seed(seed)
  u <- dqrunif(1e4, min = 10, max = 12)
  expect_true(all(u >= 10) && all(u < 12))
})

test_that("numbers can be generated in maximum range", {
  dqset.seed(seed)
  max <- .Machine$double.xmax
  u <- dqrunif(1e4, min = -max, max = max)
  expect_true(all(u >= -max) && all(u < max))
})

test_that("min and max can be equal", {
  dqset.seed(seed)
  u <- dqrunif(5, min = 10, max = 10)
  expect_true(all(u == 10))
})

test_that("min must not be larger than max", {
  dqset.seed(seed)
  expect_error(dqrunif(5, min = 10, max = 9),
               "'min' must not be larger than 'max'!")
})
