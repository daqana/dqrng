state <- new.env(parent = emptyenv())
state$RNGkind <- rep.int("default", 3)

.onLoad <- function(libname, pkgname) {
  if (getOption("dqrng.register_methods", FALSE))
    state$RNGkind <-  RNGkind("user", "user")
  else {
    if (!exists(".Random.seed", mode="numeric", envir=globalenv()))
      set.seed(NULL)
    original_seed <- get(".Random.seed", mode="numeric", envir=globalenv())
    on.exit(assign(".Random.seed", original_seed, envir=globalenv()))
    dqset_seed(seed = NULL, stream = NULL)
  }
}

.onUnload <- function(libpath) {
  if (getOption("dqrng.register_methods", FALSE))
    RNGkind(state$RNGkind[1], state$RNGkind[2], state$RNGkind[3])
}
