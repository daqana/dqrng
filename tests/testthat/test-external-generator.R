context("external-generators")

Rcpp::sourceCpp("cpp/external-generator.cpp")

n <- 1e2L
rate <- 0.4
use_seed <- 1623

test_that("external RNG (normal)", {
  dqset.seed(use_seed)
  expected <- dqrexp(n, rate)
  dqset.seed(use_seed)
  actual <- dqrexp_extrng(n, rate)
  expect_equal(actual, expected)

  dqset.seed(use_seed)
  expected2 <- expected
  actual2 <- sapply(1:n, function(x) dqrexp_extrng(1, rate))
  expect_equal(actual2, expected2)
})

test_that("external RNG (normal, Xoshiro256+)", {
  dqrng::dqRNGkind("Xoshiro256+")
  dqset.seed(use_seed)
  expected <- dqrexp(n, rate)
  dqset.seed(use_seed)
  actual <- dqrexp_extrng(n, rate)
  expect_equal(actual, expected)

  dqset.seed(use_seed)
  expected2 <- expected
  actual2 <- sapply(1:n, function(x) dqrexp_extrng(1, rate))
  expect_equal(actual2, expected2)
})

test_that("external RNG (parallel, Threefry)", {
  cl <- parallel::makeCluster(2)
  expected3 <- parallel::clusterApply(cl, 1:8, function(stream, seed, N, rate) {
    dqrng::dqRNGkind("Threefry")
    dqrng::dqset.seed(seed, stream)
    dqrng::dqrexp(N, rate)
  }, use_seed, 1e6L, rate)
  parallel::stopCluster(cl)

  cl <- parallel::makeCluster(2)
  actual3 <- parallel::clusterApply(cl, 1:8, function(stream, seed, N, rate) {
    Rcpp::sourceCpp("cpp/external-generator.cpp") ## must be recompiled
    dqrng::dqRNGkind("Threefry")
    dqrng::dqset.seed(seed, stream)
    dqrexp_extrng(N, rate)
  }, use_seed, 1e6L, rate)
  parallel::stopCluster(cl)

  expect_equal(actual3, expected3)
})
