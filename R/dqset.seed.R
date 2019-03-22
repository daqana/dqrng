#' @title R interface
#'
#' @description The \code{dqrng} package provides several fast random number
#'   generators together with fast functions for generating random numbers
#'   according to a uniform, normal and exponential distribution. These
#'   functions are modeled after the \code{base} functions
#'   \code{\link{set.seed}}, \code{\link{RNGkind}}, \code{\link{runif}},
#'   \code{\link{rnorm}}, and \code{\link{rexp}}.
#'
#' @param seed  integer scalar to seed the random number generator, or an integer vector of length 2 representing a 64-bit seed.
#' @param stream  integer used for selecting the RNG stream; either a scalar or a vector of length 2
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
#'   \item{pcg64}{The default 64 bit variant from the PCG family developed by
#'    Melissa O'Neill. See \url{http://www.pcg-random.org} for more details.}
#'   \item{Xoroshiro128+ and Xoshiro256+}{RNGs developed by David Blackman and
#'    Sebastiano Vigna. They are used as default RNGs in Erlang and Lua. See
#'     \url{http://xoshiro.di.unimi.it/} for more details.}
#'   \item{Threefry}{The 64 bit version of the 20 rounds Threefry engine as
#'    provided by \code{\link[sitmo]{sitmo-package}}}
#' }
#'
#' Xoroshiro128+ is the default since it is the fastest generator provided by
#' this package.
#'
#' The functions \code{dqrnorm} and \code{dqrexp} use the Ziggurat algorithm as
#' provided by \code{boost.random}.
#'
#' See \code{\link{generateSeedVectors}} for rapid generation of integer-vector
#' seeds that provide 64 bits of entropy. These allow full exploration of
#' the state space of the 64-bit RNGs provided in this package.
#'
#' @seealso \code{\link{set.seed}}, \code{\link{RNGkind}}, \code{\link{runif}},
#'          \code{\link{rnorm}}, and \code{\link{rexp}}
#'
#' @examples
#' library(dqrng)
#'
#' # Set custom RNG.
#' dqRNGkind("Xoshiro256+")
#'
#' # Use an integer scalar to set a seed.
#' dqset.seed(42)
#'
#' # Use integer scalars to set a seed and the stream.
#' dqset.seed(42, 123)
#'
#' # Use an integer vector to set a seed.
#' dqset.seed(c(31311L, 24123423L))
#'
#' # Use an integer vector to set a seed and a scalar to select the stream.
#' dqset.seed(c(31311L, 24123423L), 123)
#'
#' # Random sampling from distributions.
#' dqrunif(5, min = 2, max = 10)
#' dqrexp(5, rate = 4)
#' dqrnorm(5, mean = 5, sd = 3)
#' @rdname dqrng-functions
#' @export
dqset.seed <- function(seed, stream = NULL) {
  dqset_seed(seed, stream)
}
