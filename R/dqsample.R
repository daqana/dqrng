##' @title Unbiased Random Samples and Permutations
##' @param x  either a vector of one or more elements from which to choose, or a positive integer.
##' @param n  a positive number, the number of items to choose from.
##' @param size	 a non-negative integer giving the number of items to choose.
##' @param replace	should sampling be with replacement?
##' @param prob	 a vector of probability weights for obtaining the elements of the vector being sampled.
##' @seealso \code{link{sample}} and \code{\link{sample.int}}
##' @export
dqsample <- function(x, size, replace = FALSE, prob = NULL) {
    if(length(x) == 1L && is.numeric(x) && is.finite(x) && x >= 1) {
        if(missing(size)) size <- x
        dqsample.int(x, size, replace, prob)
    } else {
        if(missing(size)) size <- length(x)
        x[dqsample.int(length(x), size, replace, prob)]
    }
}

##' @rdname dqsample
##' @export
dqsample.int  <- function(n, size = n, replace = FALSE, prob = NULL) {
    if (!is.null(prob)) {
        warning("Using 'prob' is not supported yet. Using default 'sample.int'.")
        sample.int(n, size, replace, prob)
    } else if (n <= .Machine$integer.max)
        dqsample_int(n, size, replace, prob, 1L)
    else
        dqsample_num(n, size, replace, prob, 1L)
}
