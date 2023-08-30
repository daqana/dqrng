
<!-- README.md is generated from README.Rmd. Please edit that file -->

[![R build
status](https://github.com/daqana/dqrng/workflows/R-CMD-check/badge.svg)](https://github.com/daqana/dqrng/actions)
[![CRAN
status](https://www.r-pkg.org/badges/version/dqrng)](https://cran.r-project.org/package=dqrng)
[![Coverage
status](https://codecov.io/gh/daqana/dqrng/branch/master/graph/badge.svg)](https://codecov.io/github/daqana/dqrng?branch=master)
[![Downloads](https://cranlogs.r-pkg.org/badges/dqrng?color=brightgreen)](https://www.r-pkg.org/pkg/dqrng)
[![CII Best
Practices](https://bestpractices.coreinfrastructure.org/projects/2157/badge)](https://bestpractices.coreinfrastructure.org/projects/2157)
[![Dependencies](https://tinyverse.netlify.com/badge/dqrng)](https://cran.r-project.org/package=dqrng)

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
options(repos = c(
  rstub = 'https://rstub.r-universe.dev',
  CRAN = 'https://cloud.r-project.org'))
install.packages('dqrng')
```

## Example

Using the provided RNGs from R is deliberately similar to using R’s
build-in RNGs:

``` r
library(dqrng)
dqset.seed(42)
dqrunif(5, min = 2, max = 10)
#> [1] 9.211802 2.616041 6.236331 4.588535 5.764814
dqrexp(5, rate = 4)
#> [1] 0.35118613 0.17656197 0.06844976 0.16984095 0.10096744
```

They are quite a bit faster, though:

``` r
N <- 1e4
bm <- bench::mark(rnorm(N), dqrnorm(N), check = FALSE)
bm[, 1:4]
#> # A tibble: 2 × 4
#>   expression      min   median `itr/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl>
#> 1 rnorm(N)      612µs  685.2µs     1397.
#> 2 dqrnorm(N)     86µs   88.6µs    10388.
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
#> 1 sample.int(m, n, replace = TRUE)            6.88ms   7.63ms     114. 
#> 2 sample.int(1000 * m, n, replace = TRUE)     8.72ms   9.55ms      96.1
#> 3 dqsample.int(m, n, replace = TRUE)        482.21µs 810.29µs    1254. 
#> 4 dqsample.int(1000 * m, n, replace = TRUE) 492.79µs 822.86µs    1275.
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
#> 1 sample.int(m, n)                   40.1ms  42.54ms      23.5
#> 2 sample.int(1000 * m, n)           12.19ms  14.38ms      67.8
#> 3 sample.int(m, n, useHash = TRUE)   9.43ms  11.17ms      81.9
#> 4 dqsample.int(m, n)                 1.22ms   1.35ms     638. 
#> 5 dqsample.int(1000 * m, n)          1.98ms   2.51ms     358.
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
#> [1] -0.0005787967
```

## Feedback

All feedback (bug reports, security issues, feature requests, …) should
be provided as [issues](https://github.com/daqana/dqrng/issues).
