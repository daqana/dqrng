
<!-- README.md is generated from README.Rmd. Please edit that file -->
[![Travis build status](https://travis-ci.org/daqana/dqrng.svg?branch=master)](https://travis-ci.org/daqana/dqrng) [![AppVeyor build status](https://ci.appveyor.com/api/projects/status/github/daqana/dqrng?branch=master&svg=true)](https://ci.appveyor.com/project/rstub/dqrng) [![CRAN status](https://www.r-pkg.org/badges/version/dqrng)](https://cran.r-project.org/package=dqrng) [![Coverage status](https://codecov.io/gh/daqana/dqrng/branch/master/graph/badge.svg)](https://codecov.io/github/daqana/dqrng?branch=master)

dqrng
=====

The dqrng package provides fast random number generators (RNG) with good statistical properties for usage with R. It combines these RNGs with fast distribution functions to sample from uniform, normal or exponential distributions. Both the RNGs and the distribution functions are distributed as C++ header-only library.

Installation
------------

The currently released version is available from CRAN via

``` r
install.packages("dqrng")
```

Intermediate releases can also be obtained via [drat](https://cran.r-project.org/package=drat):

``` r
if (!requireNamespace("drat", quietly = TRUE)) install.packages("drat")
drat::addRepo("daqana")
install.packages("dqrng")
```

Example
-------

Using the provided RNGs from R is deliberately similar to using R's build-in RNGs:

``` r
library(dqrng)
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
dqrunif(5, min = 2, max = 10)
#> [1] 4.498747 9.647875 5.232044 6.176499 8.453008
dqrexp(5, rate = 4)
#> [1] 0.5520084 0.1982174 0.3281557 0.1913137 0.7544450
```

They are quite a bit faster, though:

``` r
N <- 1e7
system.time(rnorm(N))
#>    user  system elapsed 
#>   0.647   0.000   0.647
system.time(dqrnorm(N))
#>    user  system elapsed 
#>   0.070   0.004   0.073
```
