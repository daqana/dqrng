context("sample")

seed <- 1234567890

test_that("dqsample_int w/ replacement works", {
    dqset.seed(seed)
    result <- dqsample.int(1e5, 1e3, replace = TRUE)
    expect_equal(length(result), 1e3)
    expect_true(all(result >= 1) && all(result <= 1e5))
})

test_that("dqsample_int w/o replacement works", {
    dqset.seed(seed)
    result <- dqsample.int(1e5, 1e3, replace = FALSE)
    expect_equal(length(result), 1e3)
    expect_true(all(result >= 1) && all(result <= 1e5))
})
