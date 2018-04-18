
<!-- README.md is generated from README.Rmd. Please edit that file -->
dqrng
=====

The dqrng package provides fast random number generators (RNG) with good statistical properties for usage with R. It combines these RNGs with fast distribution functions to sample from uniform, normal or exponential distributions. Both the RNGs and the distribution functions are distributed as C++ header-only library.

Installation
------------

At the moment dqrng is not on CRAN, but you can install the current version via [drat](https://cran.r-project.org/package=drat):

``` r
if (!requireNamespace("drat", quietly = TRUE)) install.packages("drat")
drat::addRepo("RInstitute")
install.packages("dqrng")
```

Example
-------

Using the provided RNGs from R is delibratly similar to using R's build-in RNGs:

``` r
library(dqrng)
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
dqrunif(5, min = 2, max = 10)
#> [1] 4.498747 9.500660 2.457009 5.249195 2.370585
dqrexp(5, rate = 4)
#> [1] 0.4611545 0.2660513 0.3635514 0.5990732 0.5629849
```

They are quite a bit faster, though:

``` r
N <- 1e7
system.time(rnorm(N))
#>    user  system elapsed 
#>   0.658   0.008   0.667
system.time(dqrnorm(N))
#>    user  system elapsed 
#>   0.099   0.024   0.124
```
