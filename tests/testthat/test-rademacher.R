context("rademacher")

test_that("rademacher produces -1, 1", {
    out <- dqrrademacher(10)
    unique_out <- unique(out)
    expect_identical(unique_out[order(unique_out)], c(-1, 1))
})

