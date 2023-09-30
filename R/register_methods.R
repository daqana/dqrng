state <- new.env(parent = emptyenv())
state$RNGkind <- rep.int("default", 3)

#' @title Registering as user-supplied RNG
#'
#' @description The random-number generators (RNG) from this package can be
#'  registered as user-supplied RNG. This way all \code{r<dist>} functions make
#'  use of the provided fast RNGs.
#'
#' @param kind  Which methods should be registered? Either \code{"both"} or \code{"rng"}.
#'
#' @return Invisibly returns a three-element character vector of the RNG, normal
#'  and sample kinds \emph{before} the call.
#'
#' @details Caveats:
#'   \itemize{
#'     \item While \code{runif} and \code{dqrunif} as well as \code{rnorm} and
#'       \code{dqrnorm} will produce the same results, this is not the case for
#'       \code{rexp} and \code{dqrexp}.
#'     \item The \code{dqr<dist>} functions are still faster than \code{r<dist>}
#'       when many random numbers are generated.
#'     \item You can use only the RNG from this package using
#'       \code{register_method("rng")} or both the RNG and the Ziggurat method
#'       for normal draws with  \code{register_method("both")}. The latter
#'       approach is used by default. Using only the Ziggurat method will give
#'       \emph{undefined} behavior and is not supported!
#'   }
#'
#'   You can automatically register these methods when loading this package by
#'   setting the option \code{dqrng.register_methods} to \code{TRUE}, e.g.
#'   with \code{options(dqrng.register_methods=TRUE)}.
#'
#' @seealso \code{\link{RNGkind}} and \code{\link{Random.user}}
#'
#' @examples
#' register_methods()
#' # set.seed and dqset.seed influence both (dq)runif and dqrnorm
#' set.seed(4711); runif(5)
#' set.seed(4711); dqrunif(5)
#' dqset.seed(4711); rnorm(5)
#' dqset.seed(4711); dqrnorm(5)
#' # similarly for other r<dist> functions
#' set.seed(4711); rt(5, 10)
#' dqset.seed(4711); rt(5, 10)
#' # but (dq)rexp give different results
#' set.seed(4711); rexp(5, 10)
#' set.seed(4711); dqrexp(5, 10)
#' restore_methods()

#' @rdname user-supplied-rng
#' @export
register_methods <- function(kind = c("both", "rng")) {
  kind <- match.arg(kind)
  switch(kind,
         both = state$RNGkind <-  RNGkind("user", "user"),
         rng =     state$RNGkind <-  RNGkind("user"))
}

#' @rdname user-supplied-rng
#' @export
restore_methods <- function() {
  RNGkind(state$RNGkind[1], state$RNGkind[2], state$RNGkind[3])
}
