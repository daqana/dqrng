#include <Rcpp.h>
#include "R_randgen.h"

//' Generate seed as a integer vector
//'
//' @param nseeds Integer scalar, number of seeds to generate.
//' @param nwords Integer scalar, number of words to generate per seed. 
//'
//' @return
//' A list of length \code{n}, where each element is an integer vector that
//' contains \code{nwords} words (i.e., \code{32*nwords} bits) of randomness.
//'
//' @details
//' Each seed is encoded as an integer vector with the most significant bits
//' at the start of the vector. Each integer vector is converted into an
//' unsigned integer (in C++ or otherwise) by the following procedure:
//' \enumerate{
//' \item Start with a sum of zero.
//' \item Add the first value of the vector.
//' \item Left-shift the sum by 32.
//' \item Add the next value of the vector, and repeat.
//' }
//' 
//' The aim is to facilitate R-level generation of seeds with sufficient 
//' randomness to cover the entire state space of pseudo-random number 
//' generators that require more than the ~32 bits available in an  
//' \code{int}. It also preserves the integer nature of the seed, thus
//' avoiding problems with casting double-precision numbers to integers.
//'
//' It is possible for the seed vector to contain \code{NA_integer_}
//' values. This should not be cause for alarm, as R uses \code{-INT_MAX} 
//' to encode missing values in integer vectors. 
//' 
//' @author Aaron Lun
//'
//' @examples
//' generateSeedVectors(10, 2)
//'
//' generateSeedVectors(5, 4)
//'
//' @export
// [[Rcpp::export(rng = true)]]
Rcpp::List generateSeedVectors(int nseeds, int nwords=2) {
  Rcpp::List output(nseeds);
  for (int i=0; i<nseeds; ++i) {
    Rcpp::IntegerVector current(nwords);
    for (auto& val : current) {
      val = dqrng::R_random_int();
    }
    output[i]=current;
  }
  return output;
}
