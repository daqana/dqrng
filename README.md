
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
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
dqrunif(5, min = 2, max = 10)
#> [1] 8.480202 6.582408 8.869840 5.062206 8.828782
dqrexp(5, rate = 4)
#> [1] 0.39374734 0.09218016 0.15409536 0.60380418 0.06477960
```

They are quite a bit faster, though:

``` r
N <- 1e7
system.time(rnorm(N))
#>    user  system elapsed 
#>   0.755   0.024   0.779
system.time(dqrnorm(N))
#>    user  system elapsed 
#>   0.078   0.016   0.093
```
