#' @title Support for parallel computing
#'
#' @description The \code{dqrng} package provides several fast random number
#'   generators that are suited for parallel usage. The functions \code{dqrng_set_stream}
#'   and \code{dqrng_set_substream} allow to switch the RNG to a different (sub)stream.
#'   Note that substream handling is only available for the Xoshiro256+/++/**, MWC192X, and
#'   Threefry generators.
#' 
#'   The \code{dqclusterRNGStream} function prepares a \code{parallel}-type cluster for
#'   reproducible random number generation.
#'
#' @param stream  integer used for selecting the RNG stream; either a scalar or a vector of length 2
#' @param substream  integer used for selecting the RNG stream; either a scalar or a vector of length 2
#' @param cl  \code{parallel}-type cluster 
#' @param use_substream  Use different substreams instead of different streams  
#' 
#' @seealso \code{\link[parallel]{clusterSetRNGStream}} and \code{\link[parallel]{makeCluster}}
#' 
#' @examples
#' library("dqrng")
#' # get some random numbers
#' dqset.seed(123456) 
#' cl <- parallel::makePSOCKcluster(2)
#' dqclusterSetRNGStream(cl)
#' r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
#' parallel::stopCluster(cl)
#' # do the same again
#' dqset.seed(123456) 
#' cl <- parallel::makePSOCKcluster(2)
#' dqclusterSetRNGStream(cl)
#' r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
#' parallel::stopCluster(cl)
#' # make sure results are equal
#' all.equal(r1, r2)
#' 
#' @rdname parallel
#' @export
dqclusterSetRNGStream <- function(cl = NULL, use_substream = FALSE) {
    if (is.null(cl))
        cl <- parallel::getDefaultCluster()
    state <- dqrng::dqrng_get_state()
  jump_func <- dqrng::dqrng_set_stream
  if (use_substream) jump_func <- dqrng::dqrng_set_substream
    parallel::clusterApply(cl,
                           seq_along(cl) - 1,
                           function(stream, state) {
                               dqrng::dqrng_set_state(state)
                           jump_func(stream)},
                           state)
  jump_func(length(cl))
}
