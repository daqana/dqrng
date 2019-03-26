context("sample")

seed <- 1234567890

test_that("dqsample_int w/ replacement works", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample.int(n, k, replace = TRUE)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_int w/o replacement works", {
    dqset.seed(seed)
    n <- 1e5
    k <- 1e3
    result <- dqsample.int(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/ replacement works", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    n <- 1e10
    k <- 1e5
    result <- dqsample.int(n, k, replace = TRUE)
    expect_equal(length(result), k)
    expect_lte(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})

test_that("dqsample_num w/o replacement works", {
    skip_if(.Machine$sizeof.pointer <= 4, "No long-vector support")
    dqset.seed(seed)
    n <- 1e10
    k <- 1e5
    result <- dqsample.int(n, k, replace = FALSE)
    expect_equal(length(result), k)
    expect_equal(length(unique(result)), k)
    expect_true(all(result >= 1) && all(result <= n))
})
