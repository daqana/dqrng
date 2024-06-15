context("sample")

seed <- 1234567890

test_that("dqsample_int w/ replacement works for vector", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample(seq_len(n), k, replace = TRUE)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement works for vector", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample(seq_len(n), k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement works for shuffling a vector", {
    dqset.seed(seed)
    n <- 1e5
    result <- dqsample(seq_len(n), replace = FALSE)
    expect_equal(length(result), n)
    expect_equal(length(unique(result)), n)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement works for shuffling numbers", {
    dqset.seed(seed)
    n <- 1e5
    result <- dqsample(n, replace = FALSE)
    expect_equal(length(result), n)
    expect_equal(length(unique(result)), n)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/ replacement works", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample(n, k, replace = TRUE)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/ replacement works for n = 2", {
    dqset.seed(seed)
    n <- 2
    k <- 1e3
    result <- dqsample(n, k, replace = TRUE)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})


test_that("dqsample_int w/o replacement works with medium rate", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement works with low rate", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e1
    result <- dqsample(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/ replacement works", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    # use a shorter vector and internal function for performance reasons
    n <- 1e5
    k <- 1e2
    result <- dqrng:::dqsample_num(n, k, replace = TRUE, offset = 1L)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/o replacement works with high rate", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    # use a shorter vector and internal function for performance reasons
    n <- 1e5
    k <- 6e4
    result <- dqrng:::dqsample_num(n, k, replace = FALSE, offset = 1L)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/o replacement works with low rate", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    # use a shorter vector and internal function for performance reasons
    n <- 1e5
    k <- 1e2
    result <- dqrng:::dqsample_num(n, k, replace = FALSE, offset = 1L)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/o replacement works with medium rate", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    # use a shorter vector and internal function for performance reasons
    n <- 1e5
    k <- 1e4
    result <- dqrng:::dqsample_num(n, k, replace = FALSE, offset = 1)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/ replacement and w/ weights works with few sampled elements", {
  dqset.seed(seed)
  n <- 1e5
  k <- 1e3
  result <- dqsample(n, k, replace = TRUE, prob = dqrunif(n))
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/ replacement and w/ weights works with many sampled elements", {
  dqset.seed(seed)
  n <- 1e3
  k <- 1e5
  result <- dqsample(n, k, replace = TRUE, prob = dqrunif(n))
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/ replacement and w/ weights works with many sampled elements for n = 2", {
  dqset.seed(seed)
  n <- 2
  k <- 1e5
  result <- dqsample(n, k, replace = TRUE, prob = c(0.3, 0.7))
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))

  result <- dqsample(n, k, replace = TRUE, prob = c(1.3, 0.7))
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement and w/ weights works with high rate", {
  dqset.seed(seed)
  n <- 1e5
  k <- 6e4
  result <- dqsample(n, k, replace = FALSE, prob = dqrunif(n))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
  result <- dqsample(n, k, replace = FALSE, prob = c(dqrunif(n-1), 2))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement and w/ weights works with medium rate", {
  dqset.seed(seed)
  n <- 1e5
  k <- 1e3
  result <- dqsample(n, k, replace = FALSE, prob = dqrunif(n))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
  result <- dqsample(n, k, replace = FALSE, prob = c(dqrunif(n-1), 2))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement and w/ weights works with low rate", {
  dqset.seed(seed)
  n <- 1e5
  k <- 1e1
  result <- dqsample(n, k, replace = FALSE, prob = dqrunif(n))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
  result <- dqsample(n, k, replace = FALSE, prob = c(dqrunif(n-1), 2))
  expect_equal(length(result), k)
  expect_equal(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/ replacement and w/ weights works", {
  skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
  dqset.seed(seed)
  # use a shorter vector and internal function for performance reasons
  n <- 1e5
  k <- 1e2
  result <- dqrng:::dqsample_num(n, k, replace = TRUE, prob = dqrunif(n), offset = 1L)
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
  result <- dqrng:::dqsample_num(n, k, replace = TRUE, prob = c(dqrunif(n-1), 2), offset = 1L)
  expect_equal(length(result), k)
  expect_lte(length(unique(result)), k)
  expect_true(all(result >= 1) && all(result <= n))
})

test_that("error cases", {
    dqset.seed(seed)
    expect_error(dqsample(10, 20), "Argument requirements not fulfilled: n >= size")
    expect_error(dqsample(10, 20, prob = 1:10), "Argument requirements not fulfilled: n >= size")
    expect_error(dqsample(10, 20, prob = 1:20), "n == length(prob) is not TRUE", fixed = TRUE)
    expect_silent(dqsample(10, 20, replace = TRUE))
    expect_error(dqsample(10, -20), "Argument requirements not fulfilled: n > 0 && size >= 0")
    expect_error(dqsample(-10, -20), "Argument requirements not fulfilled: n > 0 && size >= 0")
    # -10 is treated as a one-element vector by dqsample but not by dqsample.int
    expect_error(dqsample(-10, 20), "Argument requirements not fulfilled: n >= size")
    expect_error(dqsample.int(-10, 20), "Argument requirements not fulfilled: n > 0 && size >= 0")

    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    expect_error(dqsample(1e10, -20), "Argument requirements not fulfilled: n > 0 && size >= 0")
})
