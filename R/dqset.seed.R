#' @title R interface
#'
#' @description The \code{dqrng} package provides several fast random number
#'   generators together with fast functions for generating random numbers
#'   according to a uniform, normal and exponential distribution. These
#'   functions are modeled after the \code{base} functions
#'   \code{\link{set.seed}}, \code{\link{RNGkind}}, \code{\link{runif}},
#'   \code{\link{rnorm}}, and \code{\link{rexp}}.
#'
#' @param seed  integer seed for the random number generator, or a raw vector representing a seed.
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
#'   \item{Mersenne-Twister}{The 64 bit variant of the well-known Mersenne-Twister,
#'    which is also used as default. This is a conservative default that allows
#'    you to take advantage of the fast distribution functions provided by dqrng
#'    while staying close to R's default RNG (32 bit Mersenne-Twister).}
#'   \item{pcg64}{The default 64 bit variant from the PCG family developed by
#'    Melissa O'Neill. See \url{http://www.pcg-random.org} for more details.}
#'   \item{Xoroshiro128+ and Xoshiro256+}{RNGs developed by David Blackman and
#'    Sebastiano Vigna. They are used as default RNGs in Erlang and Lua. See
#'     \url{http://xoshiro.di.unimi.it/} for more details.}
#'   \item{Threefry}{The 64 bit version of the 20 rounds Threefry engine as
#'    provided by \code{\link[sitmo]{sitmo-package}}}
#' }
#'
#' The functions \code{dqrnorm} and \code{dqrexp} use the Ziggurat algorithm as
#' provided by \code{boost.random}.
#'
#' @seealso \code{\link{set.seed}}, \code{\link{RNGkind}}, \code{\link{runif}},
#'          \code{\link{rnorm}}, and \code{\link{rexp}}
#'
#' @examples
#' library(dqrng)
#' dqRNGkind("Xoroshiro128+")
#' dqset.seed(42)
#' dqset.seed(generateRawSeeds(1, 64)[[1]])
#' dqrunif(5, min = 2, max = 10)
#' dqrexp(5, rate = 4)
#' dqrnorm(5, mean = 5, sd = 3)
#' @rdname dqrng-functions
#' @export
dqset.seed <- function(seed) {
  if (is.raw(seed)) {
    dqset_seed_raw(seed)
  } else {
    dqset_seed(seed)
  }
}
