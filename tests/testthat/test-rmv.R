context("multivariate generators")

seed <- 1234567890
sigma <- matrix(c(4,2,2,3), ncol=2)
mean <- c(1,2)

test_that("consecutive calls yield different random numbers (normal)", {
  skip_if_not_installed("mvtnorm")
  dqset.seed(seed)
  n1 <- dqrmvnorm(10, mean = mean, sigma=sigma)
  n2 <- dqrmvnorm(10, mean = mean, sigma=sigma)
  expect_false(all(n1 == n2))
})

test_that("setting seed produces identical normaly distributed numbers", {
  skip_if_not_installed("mvtnorm")
  dqset.seed(seed)
  n1 <- dqrmvnorm(10, mean = mean, sigma=sigma)
  dqset.seed(seed)
  n2 <- dqrmvnorm(10, mean = mean, sigma=sigma)
  expect_equal(n1, n2)
})

test_that("Means and variance as expected",{
  skip_if_not_installed("mvtnorm")
  dqset.seed(seed)
  n1 <- dqrmvnorm(1e5, mean = mean, sigma=sigma)
  expect_equal(mean, colMeans(n1), tolerance = 0.01)
  expect_equal(sigma, var(n1), tolerance = 0.01)
})
#
