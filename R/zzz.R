.onLoad <- function(libname, pkgname) {
  if (!exists(".Random.seed", mode="numeric", envir=globalenv()))
    set.seed(NULL)
  original_seed <- get(".Random.seed", mode="numeric", envir=globalenv())
  on.exit(assign(".Random.seed", original_seed, envir=globalenv()))
  dqset_seed(seed = NULL, stream = NULL)
}
