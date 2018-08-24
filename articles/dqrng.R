## ----setup, include = FALSE----------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>"
)

## ------------------------------------------------------------------------
library(dqrng)
piR <- function(N) {
    x <- dqrunif(N)
    y <- dqrunif(N)
    4 * sum(sqrt(x^2 + y^2) < 1.0) / N
}

dqRNGkind("Xoroshiro128+")
dqset.seed(42)
system.time(cat("pi ~= ", piR(10^7), "\n"))

## ------------------------------------------------------------------------
N <- 1e7
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
system.time(runif(N))
system.time(dqrng::dqrunif(N))

## ------------------------------------------------------------------------
N <- 1e7
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
system.time(rexp(N))
system.time(dqrng::dqrexp(N))

## ------------------------------------------------------------------------
N <- 1e7
dqRNGkind("Xoroshiro128+")
dqset.seed(42)
system.time(rnorm(N))
system.time(dqrng::dqrnorm(N))

