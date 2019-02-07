context("seed vector generator")

test_that("seed vector generator gives the right number of words", {
    out <- generateSeedVectors(10, nwords=2)
    expect_identical(length(out), 10L)
    expect_true(all(lengths(out)==2L))
    expect_true(all(sapply(out, typeof)=="integer"))

    out <- generateSeedVectors(50, nwords=1)
    expect_identical(length(out), 50L)
    expect_true(all(lengths(out)==1L))
    expect_true(all(sapply(out, typeof)=="integer"))

    out <- generateSeedVectors(20, nwords=4)
    expect_identical(length(out), 20L)
    expect_true(all(lengths(out)==4L))
    expect_true(all(sapply(out, typeof)=="integer"))
})

test_that("seed vector generator responds to the R seed state", {
    first <- generateSeedVectors(10, nwords=2)
    second <- generateSeedVectors(10, nwords=2)
    expect_false(identical(first, second))

    set.seed(100)
    first <- generateSeedVectors(10, nwords=2)
    set.seed(100)
    second <- generateSeedVectors(10, nwords=2)
    expect_identical(first, second)
})
