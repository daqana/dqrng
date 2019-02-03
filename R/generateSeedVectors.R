#' Generate seed as a integer vector
#'
#' @param n Integer scalar, number of seeds to generate.
#' @param bits Integer scalar, number of bits to store per seed. This
#' must be a multiple of 32.
#'
#' @return
#' A list of length \code{n}, where each element is an integer vector that
#' contains \code{bits} bits of randomness across all elements.
#'
#' @details
#' Each seed is encoded as an integer vector with the most significant bits
#' at the start of the vector. Each integer vector is converted into an
#' unsigned integer (in C++ or otherwise) by the following procedure:
#' \enumerate{
#' \item Start with a sum of zero.
#' \item Add the first value of the vector.
#' \item Left-shift the sum by 32.
#' \item Add the next value of the vector, and repeat.
#' }
#' 
#' The aim is to facilitate R-level generation of seeds with sufficient 
#' randomness to cover the entire state space of pseudo-random number 
#' generators that require more than the ~32 bits available in an  
#' \code{int}. It also preserves the integer nature of the seed, thus
#' avoiding problems with casting double-precision numbers to integers.
#'
#' It is possible for the seed vector to contain \code{NA_integer_}
#' values. This is not cause for alarm, as R uses \code{-INT_MAX} to 
#' encode missing values in integer vectors. 
#' 
#' @author Aaron Lun
#'
#' @examples
#' generateSeedVectors(10, 64)
#'
#' generateSeedVectors(5, 128)
#'
#' @export
generateSeedVectors <- function(n, bits=64) {
    bits <- as.integer(bits)
    if (bits%%32L!=0L) {
        stop("'bits' must be a multiple of 32")
    }
    dqset_create_seed_vectors(n, bits/32L) 
}

