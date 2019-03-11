context("conversion")

safe_expect_error <- function(..., msg) {
  os.type <- Sys.info()["sysname"]
  if (os.type == "Darwin") {
    expect_error(...)
  } else {
    expect_error(..., msg)
  }
}

Rcpp::sourceCpp("cpp/convert.cpp") # Warnings about shifts can be ignored.

test_that("conversion to 16-bit integers works correctly", {
    expect_identical(convert_16(0), "0")
    expect_identical(convert_16(65535), "65535")

    # Handles vectors.
    expect_identical(convert_16(c(0, 0, 0)), "0")
    expect_identical(convert_16(c(0, 65535)), "65535")
    expect_identical(convert_16(c(0, 0, 65535)), "65535")

    # Reports errors.
    skip_on_os("solaris")
    safe_expect_error(convert_16(-1), msg = "seed element out of range")
    safe_expect_error(convert_16(NA_integer_), msg = "seed element out of range")
    safe_expect_error(convert_16(65536), msg = "seed element out of range")
    safe_expect_error(convert_16(c(1, 0)), msg = "vector implies an out-of-range seed")
})

test_that("conversion to 32-bit integers works correctly", {
    expect_identical(convert_32(0), "0")
    expect_identical(convert_32(.Machine$integer.max), "2147483647")
    expect_identical(convert_32(NA_integer_), "2147483648")
    expect_identical(convert_32(-1), "4294967295")

    # Handles vectors.
    expect_identical(convert_32(c(0, 0, 0)), "0")
    expect_identical(convert_32(c(0, .Machine$integer.max)), "2147483647")
    expect_identical(convert_32(c(0, 0, -1)), "4294967295")

    # Reports errors.
    skip_on_os("solaris")
    safe_expect_error(convert_32(c(1, 0)), msg = "vector implies an out-of-range seed")
})

test_that("conversion to 64-bit integers works correctly", {
    expect_identical(convert_64(0), "0")
    expect_identical(convert_64(c(0, 0)), "0")
    expect_identical(convert_64(c(0, 0, 0)), "0")

    expect_identical(convert_64(c(0, .Machine$integer.max)), "2147483647")
    expect_identical(convert_64(c(0, 0, NA_integer_)), "2147483648")
    expect_identical(convert_64(c(0, -1)), "4294967295")
    expect_identical(convert_64(c(0, 0, NA_integer_)), "2147483648")

    expect_identical(convert_64(c(1, 0)), "4294967296")
    expect_identical(convert_64(c(1, .Machine$integer.max)), "6442450943")
    expect_identical(convert_64(c(1, NA_integer_)), "6442450944")
    expect_identical(convert_64(c(1, -1)), "8589934591")

    expect_identical(convert_64(c(-1, 0)), "18446744069414584320")
    expect_identical(convert_64(c(-1, -1)), "18446744073709551615")

    # Reports errors.
    skip_on_os("solaris")
    safe_expect_error(convert_64(c(1, 1, 0)), msg = "vector implies an out-of-range seed")
})

test_that("unsigned/signed methods are consistent", {
    extremes <- c(0L, 1L, .Machine$integer.max, NA_integer_, -1L)
    for (x in extremes) {
        for (y in extremes) {
            expect_true(is_signed_consistent(c(x, y)))
        }
    }
})
