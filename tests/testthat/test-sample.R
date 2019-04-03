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

test_that("dqsample_int w/o replacement works", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
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

test_that("dqsample_num w/o replacement works", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    n <- 1e10
    k <- 1e5
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
