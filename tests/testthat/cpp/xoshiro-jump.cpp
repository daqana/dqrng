#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include <xoshiro.h>
// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
bool xoroshiro_jump() {
  dqrng::xoroshiro128plus rng1;
  dqrng::xoroshiro128plus rng2{rng1};
  if (rng1() != rng2()) return false;
  rng2.jump(1);
  return (rng1() != rng2());
}

// [[Rcpp::export]]
bool xoshiro_jump() {
  dqrng::xoshiro256plus rng1;
  dqrng::xoshiro256plus rng2{rng1};
  if (rng1() != rng2()) return false;
  rng2.jump(1);
  return (rng1() != rng2());
}
