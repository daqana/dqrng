
<!-- README.md is generated from README.Rmd. Please edit that file -->

[![Travis build
status](https://travis-ci.org/daqana/dqrng.svg?branch=master)](https://travis-ci.org/daqana/dqrng)
[![AppVeyor build
status](https://ci.appveyor.com/api/projects/status/github/daqana/dqrng?branch=master&svg=true)](https://ci.appveyor.com/project/rstub/dqrng)
[![CRAN
status](https://www.r-pkg.org/badges/version/dqrng)](https://cran.r-project.org/package=dqrng)
[![Coverage
status](https://codecov.io/gh/daqana/dqrng/branch/master/graph/badge.svg)](https://codecov.io/github/daqana/dqrng?branch=master)
[![Downloads](http://cranlogs.r-pkg.org/badges/dqrng?color=brightgreen)](http://www.r-pkg.org/pkg/dqrng)
[![CII Best
Practices](https://bestpractices.coreinfrastructure.org/projects/2157/badge)](https://bestpractices.coreinfrastructure.org/projects/2157)
[![Codacy
Badge](https://api.codacy.com/project/badge/Grade/cc38be1f5900415a84010acd9ef85ce1)](https://www.codacy.com/app/rstub/dqrng?utm_source=github.com&utm_medium=referral&utm_content=daqana/dqrng&utm_campaign=Badge_Grade)

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
[drat](https://cran.r-project.org/package=drat):

``` r
if (!requireNamespace("drat", quietly = TRUE)) install.packages("drat")
drat::addRepo("daqana")
install.packages("dqrng")
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
bm[, 1:5]
#> # A tibble: 2 x 5
#>   expression      min     mean   median      max
#>   <chr>      <bch:tm> <bch:tm> <bch:tm> <bch:tm>
#> 1 rnorm(N)      594µs  702.6µs  684.7µs   1.03ms
#> 2 dqrnorm(N)   57.7µs   71.5µs   69.5µs 189.16µs
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
bm[, 1:5]
#> # A tibble: 4 x 5
#>   expression                                  min     mean  median      max
#>   <chr>                                  <bch:tm> <bch:tm> <bch:t> <bch:tm>
#> 1 sample.int(m, n, replace = TRUE)       904.62µs   1.01ms 981.3µs   1.81ms
#> 2 sample.int(1000 * m, n, replace = TRU…   1.69ms   1.85ms   1.8ms   2.75ms
#> 3 dqsample.int(m, n, replace = TRUE)     294.47µs 335.16µs 328.8µs 514.47µs
#> 4 dqsample.int(1000 * m, n, replace = T… 314.42µs 363.91µs 358.5µs 568.85µs
```

And without replacement:

``` r
bm <- bench::mark(sample.int(m, n),
                  sample.int(1e3*m, n),
                  sample.int(m, n, useHash = TRUE),
                  dqsample.int(m, n),
                  dqsample.int(1e3*m, n),
                  check = FALSE)
bm[, 1:5]
#> # A tibble: 5 x 5
#>   expression                            min     mean   median      max
#>   <chr>                            <bch:tm> <bch:tm> <bch:tm> <bch:tm>
#> 1 sample.int(m, n)                  19.02ms  19.62ms  19.43ms  20.41ms
#> 2 sample.int(1000 * m, n)            5.25ms   6.12ms   5.57ms   9.31ms
#> 3 sample.int(m, n, useHash = TRUE)   3.39ms   3.81ms   3.52ms   6.97ms
#> 4 dqsample.int(m, n)                  1.2ms   1.52ms   1.38ms   4.66ms
#> 5 dqsample.int(1000 * m, n)           1.7ms   2.16ms   1.95ms   4.85ms
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
