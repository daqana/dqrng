
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
N <- 1e7
system.time(rnorm(N))
#>    user  system elapsed 
#>   0.776   0.012   0.790
system.time(dqrnorm(N))
#>    user  system elapsed 
#>   0.088   0.008   0.098
```

## Feedback

All feedback (bug reports, security issues, feature requests, …) should
be provided as [issues](https://github.com/daqana/dqrng/issues).
