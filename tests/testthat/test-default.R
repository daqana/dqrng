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

test_that("setting seed produces identical normaly distributed numbers", {
  dqset.seed(seed)
  n1 <- dqrnorm(10)
  dqset.seed(seed)
  n2 <- dqrnorm(10)
  expect_equal(n1, n2)
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
