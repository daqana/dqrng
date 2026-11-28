#' @title Support for parallel computing
#'
#' @description The \code{dqrng} package provides several fast random number
#'   generators that are suited for parallel usage. The function \code{dqrng_advance_stream}
#'   allows to switch the RNG to a different (sub)stream. Note that substream handling via
#'   the option \code{use_substream} is only available for the Xoshiro256+/++/**, MWC192X, and
#'   Threefry generators.
#' 
#'   The \code{dqclusterRNGStream} function prepares a \code{parallel}-type cluster for
#'   reproducible random number generation.
#'
#' @param stream  integer used for selecting the RNG stream; either a scalar or a vector of length 2
#' @param cl  \code{parallel}-type cluster
#' @param use_substream  Use different substreams instead of different streams
#' @param register  Should the RNG be registered as \code{user-defined}, default \code{TRUE}  
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
dqclusterSetRNGStream <- function(cl = NULL, use_substream = FALSE, register = TRUE) {
   if (is.null(cl))
    cl <- parallel::getDefaultCluster()
  state <- dqrng::dqrng_get_state()
  parallel::clusterApply(cl,
                         seq_along(cl) - 1,
                         function(stream, state, use_substream) {
                           if (register) dqrng::register_methods()
                           dqrng::dqrng_set_state(state)
                           dqrng::dqrng_advance_stream(stream, use_substream)},
                         state,
                         use_substream)
  dqrng::dqrng_advance_stream(length(cl), use_substream)
}
