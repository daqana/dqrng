#' Generate seed as a raw vector
#'
#' @param n Integer scalar, number of seeds to generate.
#' @param bits Integer scalar, number of bits to store per seed.
#'
#' @return
#' A list of length \code{n}, where each element is a raw vector that
#' contains \code{bits} bits of randomness.
#'
#' @details
#' Each seed is encoded as a raw vector with the most significant bits
#' at the start of the vector. Each raw vector is converted into an
#' unsigned integer by the following procedure:
#' \enumerate{
#' \item Start with a sum of zero.
#' \item Add the first value of the vector.
#' \item Left-shift the sum by 8.
#' \item Add the next value of the vector, and repeat.
#' }
#' 
#' The aim is to facilitate R-level generation of seeds with sufficient 
#' randomness to cover the entire state space of pseudo-random number 
#' generators that require more than the ~32 bits available in a typical
#' \code{int}. It also preserves the integer nature of the seed, thus
#' avoiding problems with casting double-precision numbers to integers.
#' 
#' @author Aaron Lun
#'
#' @examples
#' generateRawSeeds(10, 64)
#'
#' generateRawSeeds(5, 128)
#'
#' generateRawSeeds(10, 50) # does not need to be divisible by 8.
#'
#' @export
generateRawSeeds <- function(n, bits) {
    bits <- as.integer(bits)
    nvals <- ceiling(bits/8L)
    if (nvals==0L) {
        return(raw(n))
    }

    collected <- vector("list", nvals)
    while (bits > 0L) {
        current <- 2L^min(bits, 8L)
        choices <- all_values[seq_len(current)]
        collected[[nvals]] <- sample(choices, n, replace=TRUE)
        bits <- bits - 8L
        nvals <- nvals - 1L
    }

    do.call(mapply, c(collected, list(FUN=c, SIMPLIFY=FALSE)))
}

all_values <- as.raw(seq_len(2L^8L) - 1L)
