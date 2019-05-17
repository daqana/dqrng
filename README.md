
<!-- README.md is generated from README.Rmd. Please edit that file -->

[![Travis build
status](https://travis-ci.org/daqana/dqrng.svg?branch=master)](https://travis-ci.org/daqana/dqrng)
[![AppVeyor build
status](https://ci.appveyor.com/api/projects/status/github/daqana/dqrng?branch=master&svg=true)](https://ci.appveyor.com/project/rstub/dqrng)
[![CRAN
status](https://www.r-pkg.org/badges/version/dqrng)](https://cran.r-project.org/package=dqrng)
[![Coverage
status](https://codecov.io/gh/daqana/dqrng/branch/master/graph/badge.svg)](https://codecov.io/github/daqana/dqrng?branch=master)
[![Downloads](https://cranlogs.r-pkg.org/badges/dqrng?color=brightgreen)](https://www.r-pkg.org/pkg/dqrng)
[![CII Best
Practices](https://bestpractices.coreinfrastructure.org/projects/2157/badge)](https://bestpractices.coreinfrastructure.org/projects/2157)
[![Codacy
Badge](https://api.codacy.com/project/badge/Grade/cc38be1f5900415a84010acd9ef85ce1)](https://www.codacy.com/app/rstub/dqrng?utm_source=github.com&utm_medium=referral&utm_content=daqana/dqrng&utm_campaign=Badge_Grade)
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
bm[, 1:4]
#> # A tibble: 2 x 4
#>   expression      min   median `itr/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl>
#> 1 rnorm(N)      630µs  720.7µs     1366.
#> 2 dqrnorm(N)   71.3µs   80.9µs    11897.
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
#> # A tibble: 4 x 4
#>   expression                                     min   median `itr/sec`
#>   <bch:expr>                                <bch:tm> <bch:tm>     <dbl>
#> 1 sample.int(m, n, replace = TRUE)            5.93ms   6.39ms      153.
#> 2 sample.int(1000 * m, n, replace = TRUE)     7.26ms   7.85ms      127.
#> 3 dqsample.int(m, n, replace = TRUE)        288.92µs 339.85µs     2774.
#> 4 dqsample.int(1000 * m, n, replace = TRUE) 346.69µs 379.22µs     2369.
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
#> # A tibble: 5 x 4
#>   expression                            min   median `itr/sec`
#>   <bch:expr>                       <bch:tm> <bch:tm>     <dbl>
#> 1 sample.int(m, n)                  34.22ms  36.02ms      26.0
#> 2 sample.int(1000 * m, n)           11.99ms  12.97ms      72.6
#> 3 sample.int(m, n, useHash = TRUE)    9.3ms  10.09ms      92.7
#> 4 dqsample.int(m, n)                 1.34ms   1.49ms     596. 
#> 5 dqsample.int(1000 * m, n)          1.69ms   2.03ms     434.
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
