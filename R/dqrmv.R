#' @title Multivariate Distributions
#'
#' @param n  number of  observations
#' @param ... forwarded to \code{\link[mvtnorm]{rmvnorm}}
#'
#' @return numeric matrix of multivariate normal distributed variables
#' @seealso \code{\link[mvtnorm]{rmvnorm}}
#'
#' @export
#'
#' @rdname dqrmv
#' @examples
#' sigma <- matrix(c(4,2,2,3), ncol=2)
#' x <- dqrmvnorm(n=500, mean=c(1,2), sigma=sigma)
#' colMeans(x)
#' var(x)
#' plot(x)
dqrmvnorm <- function(n, ...) {
  if (!requireNamespace("mvtnorm", quietly = TRUE)) {
    stop("Package \"mvtnorm\" must be installed to use this function.", call. = FALSE)
  }
  mvtnorm::rmvnorm(n, ..., rnorm = dqrnorm)
}
