
<!-- README.md is generated from README.Rmd. Please edit that file -->

[![R build
status](https://github.com/daqana/dqrng/workflows/R-CMD-check/badge.svg)](https://github.com/daqana/dqrng/actions)
[![CRAN
status](https://www.r-pkg.org/badges/version/dqrng)](https://cran.r-project.org/package=dqrng)
[![Coverage
status](https://codecov.io/gh/daqana/dqrng/branch/main/graph/badge.svg)](https://app.codecov.io/github/daqana/dqrng?branch=main)
[![Downloads](https://cranlogs.r-pkg.org/badges/dqrng?color=brightgreen)](https://www.r-pkg.org/pkg/dqrng)
[![CII Best
Practices](https://bestpractices.coreinfrastructure.org/projects/2157/badge)](https://bestpractices.coreinfrastructure.org/projects/2157)
[![Dependencies](https://tinyverse.netlify.com/badge/dqrng)](https://cran.r-project.org/package=dqrng)
[![dqrng status
badge](https://rstub.r-universe.dev/badges/dqrng)](https://rstub.r-universe.dev/dqrng)

# dqrng

The dqrng package provides fast random number generators (RNG) with good
statistical properties for usage with R. It combines these RNGs with
fast distribution functions to sample from uniform, normal or
exponential distributions. Both the RNGs and the distribution functions
are distributed as C++ header-only library.

## Installation

The currently released version is available from CRAN via

``` r
install.packages("dqrng")
```

Intermediate releases can also be obtained via
[r-universe](https://rstub.r-universe.dev/dqrng):

``` r
install.packages('dqrng', repos = c(
  rstub = 'https://rstub.r-universe.dev',
  CRAN = 'https://cloud.r-project.org'))
```

## Example

Using the provided RNGs from R is deliberately similar to using R’s
build-in RNGs:

``` r
library(dqrng)
dqset.seed(42)
dqrunif(5, min = 2, max = 10)
#> [1] 9.266963 4.644899 9.607483 3.635770 4.742639
dqrexp(5, rate = 4)
#> [1] 0.111103883 0.084289794 0.003414377 0.042012033 0.143914583
```

They are quite a bit faster, though:

``` r
N <- 1e4
bm <- bench::mark(rnorm(N), dqrnorm(N), check = FALSE)
bm[, 1:4]
#> # A tibble: 2 × 4
#>   expression      min   median `itr/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl>
#> 1 rnorm(N)    606.2µs  654.7µs     1456.
#> 2 dqrnorm(N)   82.8µs   85.9µs    10984.
```

This is also true for the provided sampling functions with replacement:

``` r
m <- 1e7
n <- 1e5
bm <- bench::mark(sample.int(m, n, replace = TRUE),
                  sample.int(1e3*m, n, replace = TRUE),
                  dqsample.int(m, n, replace = TRUE),
                  dqsample.int(1e3*m, n, replace = TRUE),
                  check = FALSE)
bm[, 1:4]
#> # A tibble: 4 × 4
#>   expression                                     min   median `itr/sec`
#>   <bch:expr>                                <bch:tm> <bch:tm>     <dbl>
#> 1 sample.int(m, n, replace = TRUE)            6.88ms   7.07ms      140.
#> 2 sample.int(1000 * m, n, replace = TRUE)     8.57ms   8.81ms      112.
#> 3 dqsample.int(m, n, replace = TRUE)        289.69µs 296.86µs     2834.
#> 4 dqsample.int(1000 * m, n, replace = TRUE) 407.45µs 489.33µs     1645.
```

And without replacement:

``` r
bm <- bench::mark(sample.int(m, n),
                  sample.int(1e3*m, n),
                  sample.int(m, n, useHash = TRUE),
                  dqsample.int(m, n),
                  dqsample.int(1e3*m, n),
                  check = FALSE)
#> Warning: Some expressions had a GC in every iteration; so filtering is
#> disabled.
bm[, 1:4]
#> # A tibble: 5 × 4
#>   expression                            min   median `itr/sec`
#>   <bch:expr>                       <bch:tm> <bch:tm>     <dbl>
#> 1 sample.int(m, n)                  40.98ms   42.8ms      23.1
#> 2 sample.int(1000 * m, n)           12.01ms   13.3ms      66.9
#> 3 sample.int(m, n, useHash = TRUE)   9.35ms   10.4ms      92.4
#> 4 dqsample.int(m, n)               616.34µs  679.1µs    1265. 
#> 5 dqsample.int(1000 * m, n)          1.42ms    1.7ms     501.
```

Note that sampling from `10^10` elements triggers “long-vector support”
in R.

In addition the RNGs provide support for multiple independent streams
for parallel usage:

``` r
N <- 1e7
dqset.seed(42, 1)
u1 <- dqrunif(N)
dqset.seed(42, 2)
u2 <- dqrunif(N)
cor(u1, u2)
#> [1] 0.0009574617
```

It is also possible to register the supplied generators as user-supplied
RNGs. This way `set.seed()` and `dqset.seed()` influence both
`(dq)runif` and `(dq)rnorm` in the same way. This is also true for other
`r<dist>` functions, but note that `rexp` and `dqrexp` still give
different results:

``` r
register_methods()
set.seed(4711); runif(5)
#> [1] 0.3143534 0.7835753 0.1443660 0.1109871 0.6433407
set.seed(4711); dqrunif(5)
#> [1] 0.3143534 0.7835753 0.1443660 0.1109871 0.6433407
dqset.seed(4711); rnorm(5)
#> [1] -0.3618122  0.8199887 -0.4075635  0.2073972 -0.8038326
dqset.seed(4711); dqrnorm(5)
#> [1] -0.3618122  0.8199887 -0.4075635  0.2073972 -0.8038326
set.seed(4711); rt(5, 10)
#> [1] -0.3196113 -0.4095873 -1.2928241  0.2399470 -0.1068945
dqset.seed(4711); rt(5, 10)
#> [1] -0.3196113 -0.4095873 -1.2928241  0.2399470 -0.1068945
set.seed(4711); rexp(5, 10)
#> [1] 0.0950560698 0.0567150561 0.1541222748 0.2512966671 0.0002175758
set.seed(4711); dqrexp(5, 10)
#> [1] 0.03254731 0.06855303 0.06977124 0.02579004 0.07629535
restore_methods()
```

## Feedback

All feedback (bug reports, security issues, feature requests, …) should
be provided as [issues](https://github.com/daqana/dqrng/issues).
