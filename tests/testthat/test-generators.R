context("generators")

seed <- 1234567890

test_that("Mersenne-Twister: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Mersenne-Twister")
  dqset.seed(seed)
  u1 <- dqrunif(10)
  dqset.seed(seed)
  u2 <- dqrunif(10)
  expect_equal(u1, u2)
})

test_that("PCG64: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("pcg64")
  dqset.seed(seed)
  u1 <- dqrunif(10)
  dqset.seed(seed)
  u2 <- dqrunif(10)
  expect_equal(u1, u2)
})

test_that("Xoroshiro128: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Xoroshiro128+")
  dqset.seed(seed)
  u1 <- dqrunif(10)
  dqset.seed(seed)
  u2 <- dqrunif(10)
  expect_equal(u1, u2)
})

test_that("Xoshiro256: setting seed produces identical uniformly distributed numbers", {
  dqRNGkind("Xoshiro256+")
  dqset.seed(seed)
  u1 <- dqrunif(10)
  dqset.seed(seed)
  u2 <- dqrunif(10)
  expect_equal(u1, u2)
})

test_that("non-existant RNG produces error", {
  expect_error(dqRNGkind("does_not_exist"))
})
