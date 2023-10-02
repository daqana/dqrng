#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include <xoshiro.h>
// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
bool xoroshiro_plus_jump() {
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
bool xoroshiro_plus_plus_jump() {
  dqrng::xoroshiro128plusplus rng1;
  dqrng::xoroshiro128plusplus rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 9619973171922872170ul) return false;
  rng2.jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 18140424698325186121ul && b2 == 4303743122785247830ul);
}

// [[Rcpp::export]]
bool xoroshiro_star_star_jump() {
  dqrng::xoroshiro128starstar rng1;
  dqrng::xoroshiro128starstar rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 9411227923535470089ul) return false;
  rng2.jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 13565053679963375829ul && b2 == 16626251538516947336ul);
}

// [[Rcpp::export]]
bool xoshiro_plus_long_jump() {
  dqrng::xoshiro256plus rng1;
  dqrng::xoshiro256plus rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 1636916528417697424ul) return false;
  rng2.long_jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 7650610156038910584ul && b2 == 3829589126310379935ul);
}

// [[Rcpp::export]]
bool xoshiro_plus_plus_long_jump() {
  dqrng::xoshiro256plusplus rng1;
  dqrng::xoshiro256plusplus rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 1043556088660792683ul) return false;
  rng2.long_jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 17143711001946577550ul && b2 == 5651347275477359870ul);
}

// [[Rcpp::export]]
bool xoshiro_star_star_long_jump() {
  dqrng::xoshiro256starstar rng1;
  dqrng::xoshiro256starstar rng2{rng1};
  uint64_t b1 = rng1();
  uint64_t b2 = rng2();
  if (b1 != b2 || b1 != 2395286222372858216) return false;
  rng2.long_jump();
  b1 = rng1();
  b2 = rng2();
  return (b1 != b2 && b1 == 16934533178094428553ul && b2 == 2234110801786820680ul);
}
