#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include <mwc192x.h>
// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
bool mwc192x_jump() {
  dqrng::mwc192x rng1;
  dqrng::mwc192x rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 12249618172548589915ul) return false;
  rng2.jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 17433566512221114066ul && b2 == 12384328017131331964ul);
}

// [[Rcpp::export]]
bool mwc192x_long_jump() {
  dqrng::mwc192x rng1;
  dqrng::mwc192x rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 12249618172548589915ul) return false;
  rng2.long_jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 17433566512221114066ul && b2 == 15433783979612660550ul);
}

// [[Rcpp::export]]
bool mwc192x_jump_n() {
  dqrng::mwc192x rng1;
  dqrng::mwc192x rng2{rng1};
  rng1.jump();
  rng1.jump();
  rng1.jump();
  rng1.long_jump();
  rng1.long_jump();
  rng2.jump(3);
  rng2.long_jump(2);
  return rng1() == rng2();
}
