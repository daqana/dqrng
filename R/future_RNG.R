is_xoshiro256pp_seed <- function(seed) {
  is.character(seed) &&
    length(seed) == 5L &&
    seed[1] == "xoshiro256++" &&
    grep("^[0-9]+$", seed, invert = TRUE) == 1
}

as_xoshiro256pp_seed <- function(seed) {
  ## Generate a Xoshiro256++ seed (existing or random)?
  if (is.logical(seed)) {
    if (length(seed) != 1L && !is.na(seed) && !seed) {
      stop("Argument 'seed' must be TRUE if logical: %s", seed)
    }

    oseed <- dqrng_get_state()
    
    ## Already a Xoshiro256++ seed?  Then use that as is.
    if (!is.na(seed) && seed) {
      if (is_xoshiro256pp_seed(oseed)) return(oseed)
    }

    ## Make sure to not forward the RNG state or the RNG kind
    on.exit(dqrng_set_state(oseed), add = TRUE)

    ## Generate a random Xoshiro256++ seed from the current RNG state
    dqRNGkind("Xoshiro256++")
    
    return(dqrng_get_state())
  }

  ## Already a Xoshiro256++ seed?
  if (is_xoshiro256pp_seed(seed)) {
    return(seed)
  }

  ## Generate a new Xoshiro256++ seed?
  if (is.numeric(seed) && all(is.finite(seed)) && length(seed) <= 2) {
    seed <- as.integer(seed)

    ## Generate a random Xoshiro256++ seed from the current RNG state
    oseed <- dqrng_get_state()
    
    ## Make sure to not forward the RNG state or the RNG kind
    on.exit(dqrng_set_state(oseed), add = TRUE)
    
    ## ... based on 'seed'
    dqRNGkind("Xoshiro256++")
    dqset.seed(seed)
    return(dqrng_get_state())
  }
  
  stop("Argument 'seed' must be TRUE, Xoshiro256++ RNG state as returned by dqrng_get_state() or an integer vector with length <= 2")
}
