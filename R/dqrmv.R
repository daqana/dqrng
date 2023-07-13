#' @title Multivariate Distributions
#'
#' @param n  number of  observations
#' @param ... forwarded to \code{\link[mvtnorm]{rmvnorm}} or \code{\link[mvtnorm]{rmvt}}
#'
#' @return numeric matrix of multivariate normal or t-distributed variables
#' @seealso \code{\link[mvtnorm]{rmvnorm}} and \code{\link[mvtnorm]{rmvt}}
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
    stop(
      "Package \"mvtnorm\" must be installed to use this function.",
      call. = FALSE
    )
  }
  mvtnorm::rmvnorm(n, ..., rnorm = dqrnorm)
}

#' @export
#'
#' @rdname dqrmv
#' @examples
#' ## X ~ t_3(0, diag(2))
#' x <- dqrmvt(100, sigma = diag(2), df = 3) # t_3(0, diag(2)) sample
#' plot(x)
dqrmvt <- function(n, ...) {
  if (!requireNamespace("mvtnorm", quietly = TRUE)) {
    stop(
      "Package \"mvtnorm\" must be installed to use this function.",
      call. = FALSE
    )
  }
  mvtnorm::rmvt(n, ..., rnorm = dqrnorm)
}
