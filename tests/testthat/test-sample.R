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
    n <- 1e10
    k <- 1e5
    result <- dqsample(n, k, replace = TRUE)
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
    n <- 1e10
    k <- 1e5
    result <- dqsample(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/o replacement works with medium rate", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    n <- 1e10
    k <- 1e8
    result <- dqsample(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("sampling with weights gives warning", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    expect_warning(dqsample(n, k, replace = TRUE, prob = dqrunif(n)),
                   "Using 'prob' is not supported yet. Using default 'sample.int'.")
})

test_that("error cases", {
    dqset.seed(seed)
    expect_error(dqsample(10, 20), "Argument requirements not fulfilled: m >= n")
    expect_silent(dqsample(10, 20, replace = TRUE))
    expect_error(dqsample(10, -20), "Argument requirements not fulfilled: m > 0 && n >= 0")
    expect_error(dqsample(-10, -20), "Argument requirements not fulfilled: m > 0 && n >= 0")
    # -10 is treated as a one-element vector by dqsample but not by dqsample.int
    expect_error(dqsample(-10, 20), "Argument requirements not fulfilled: m >= n")
    expect_error(dqsample.int(-10, 20), "Argument requirements not fulfilled: m > 0 && n >= 0")

    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    expect_error(dqsample(1e10, -20), "Argument requirements not fulfilled: m > 0 && n >= 0")
})
