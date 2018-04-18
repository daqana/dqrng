#' Fast Pseudo Random Number Generators for R
#'
#' This package provides several fast random number generators as C++
#' header only libraries. In addition fast functions for generating random
#' numbers according to a uniform, normal and exponential distribution are
#' provided. These functions are exported to R and as a C++ interface.
#'
#' @name dqrng
#' @docType package
#' @useDynLib dqrng, .registration = TRUE
#' @importFrom Rcpp evalCpp
NULL

#' @title dqrng functions
#' @name dqrng functions
#'
#' @description The \code{dqrng} package provides function for random number generation.
#' These functions are modeled after the \code{base} functions \code{\link{set.seed}},
#' \code{\link{RNGkind}}, \code{\link{runif}}, \code{\link{rnorm}}, and \code{\link{rexp}}.
#'
#' @param seed  integer seed for the random number generator
#' @param kind  string specifying the RNG (see details)
#' @param normal_kind  ignored; included for compatibility with \code{\link{RNGkind}}
#' @param n  number of  observations
#' @param min  lower limit of the uniform distribution
#' @param max  upper limit of the uniform distribution
#' @param mean  mean value of the normal distribution
#' @param sd  standard deviation of the normal distribution
#' @param rate  rate of the exponential distribution
#'
#' @return \code{dqrunif}, \code{dqrnorm}, and \code{dqrexp} return a numeric vector of length \code{n}.
#'
#' @details Supported RNG kinds:
#' \describe{
#'   \item{Mersenne-Twister}{The 64bit variant of Mersenne-Twister. Also the default.}
#'   \item{pcg64}{\url{http://www.pcg-random.org}}
#'   \item{Xorshift128+}{\url{http://xoroshiro.di.unimi.it/}}
#'   \item{Xorshift1024*}{\url{http://xoroshiro.di.unimi.it/}}
#'   \item{Xoroshiro128+}{\url{http://xoroshiro.di.unimi.it/}}
#' }
#'
#' \code{rnorm} and \code{rexp} use the Ziggurat method as provided by \code{boost.random}.
#'
#' @seealso \code{\link{set.seed}}, \code{\link{RNGkind}}, \code{\link{runif}},
#'          \code{\link{rnorm}}, and \code{\link{rexp}}
NULL
