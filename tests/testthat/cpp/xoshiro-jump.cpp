#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include <xoshiro.h>
// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
bool xoroshiro_jump() {
  dqrng::xoroshiro128plus rng1;
  dqrng::xoroshiro128plus rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 8565694473480078012ul) return false;
  rng2.jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 15272923827117731108ul && b2 == 17999886805244497548ul);
}

// [[Rcpp::export]]
bool xoshiro_jump() {
  dqrng::xoshiro256plus rng1;
  dqrng::xoshiro256plus rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 1636916528417697424ul) return false;
  rng2.jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 != 15445422994140413325ul && b2 != 10032410244159205089ul);
}
