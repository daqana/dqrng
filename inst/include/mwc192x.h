/*
 Written in 2023-2026 by Sebastiano Vigna (vigna@acm.org)

 To the extent possible under law, the author has dedicated all copyright
 and related and neighboring rights to this software to the public domain
 worldwide. This software is distributed without any warranty.

 See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#ifndef MWC192X_H
#define MWC192X_H 1

#include <array>
#include <mystdint.h>
#include <functional>
#include <iostream>

namespace dqrng {

/* This is MWC192X 1.0, a scrambled multiply-with-carry generator with
 64-bit digits, lag two, and period approximately 2^191, from "Scrambled
 Multiply-with-Carry Pseudorandom Number Generators" by Sebastiano Vigna
 (https://arxiv.org/abs/2610.27313). It is one of the fastest generators
 available, and it passes all tests we are aware of.

 The output is the most recent digit xored with the carry (scrambler X).
 The underlying generator simulates a multiplicative congruential generator
 with prime modulus m = 0xff4e24dd418f5433 * 2^128 - 1 and multiplier
 2^-64 mod m, so jumping ahead by n steps is a multiplication of the state
 x + y * 2^64 + c * 2^128 by a power of the multiplier modulo m: jump(n)
 and long_jump(n) take time logarithmic in n.

 The state must be seeded so that 0 < c < 0xff4e24dd418f5432. The seed()
 functions initialize x, y, and c to valid values using the output of the
 given function, or of a splitmix64 generator seeded with the given seed. */

class mwc192x {
public:
  using result_type = uint64_t;

private:
  static constexpr uint64_t MWC_A2 = 0xff4e24dd418f5433ULL;
  using words = std::array<uint64_t, 3>;

  uint64_t x, y, c;

  struct SplitMix {
    SplitMix(const uint64_t& k) : state(k) {}

    uint64_t operator() () {
      uint64_t z = (state += 0x9e3779b97f4a7c15ULL);
      z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
      z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
      return z ^ (z >> 31);
    }

  private:
    uint64_t state;
  };

  // Returns the lower 64 bits of a * x + c and stores the upper 64 bits in hi.
  static uint64_t mul_add(const uint64_t a, const uint64_t x, const uint64_t c, uint64_t& hi) {
#if defined(__SIZEOF_INT128__)
    __extension__ typedef unsigned __int128 uint128_t;
    const uint128_t t = static_cast<uint128_t>(a) * x + c;
    hi = static_cast<uint64_t>(t >> 64);
    return static_cast<uint64_t>(t);
#else
    // Schoolbook multiplication on 32-bit halves; each partial product can
    // absorb two 32-bit addends without overflow.
    const uint64_t mask = 0xffffffffULL;
    const uint64_t a0 = a & mask, a1 = a >> 32, x0 = x & mask, x1 = x >> 32;
    const uint64_t t0 = a0 * x0 + (c & mask);
    const uint64_t t1 = a0 * x1 + (t0 >> 32) + (c >> 32);
    const uint64_t t2 = a1 * x0 + (t1 & mask);
    hi = a1 * x1 + (t1 >> 32) + (t2 >> 32);
    return (t2 << 32) | (t0 & mask);
#endif
  }

  // Minimal modular arithmetic on little-endian 192-bit numbers smaller than
  // m, used only by the jump functions. Multiplication is shift-and-add, which
  // needs no wide multiplication: it is slow, but jumps are rare.

  static words modulus() {
    return words{{0xffffffffffffffffULL, 0xffffffffffffffffULL, MWC_A2 - 1}};
  }

  static bool ge(const words& a, const words& b) {
    for (int i = 2; i >= 0; --i)
      if (a[i] != b[i]) return a[i] > b[i];
    return true;
  }

  // a = (a + b) mod m
  static void add_mod(words& a, const words& b) {
    const words m = modulus();
    uint64_t carry = 0;
    for (int i = 0; i < 3; ++i) {
      const uint64_t s = a[i] + b[i];
      const uint64_t c0 = s < a[i];
      a[i] = s + carry;
      carry = c0 | (a[i] < s);
    }
    // The sum is smaller than 2m, so one subtraction suffices; if the sum
    // overflowed, the subtraction wraps around to the correct result.
    if (carry != 0 || ge(a, m)) {
      uint64_t borrow = 0;
      for (int i = 0; i < 3; ++i) {
        const uint64_t d = a[i] - m[i];
        const uint64_t b0 = d > a[i];
        a[i] = d - borrow;
        borrow = b0 | (a[i] > d);
      }
    }
  }

  // Returns a * b mod m
  static words mul_mod(const words& a, const words& b) {
    words r{{0, 0, 0}};
    words t = a;
    for (int i = 0; i < 192; ++i) {
      if ((b[i / 64] >> (i % 64)) & 1) add_mod(r, t);
      const words u = t;
      add_mod(t, u);
    }
    return r;
  }

  // Returns b^n mod m
  static words pow_mod(words b, uint64_t n) {
    words r{{1, 0, 0}};
    while (n != 0) {
      if (n & 1) r = mul_mod(r, b);
      n >>= 1;
      if (n != 0) b = mul_mod(b, b);
    }
    return r;
  }

  // Multiplies the state by r modulo m.
  void do_jump(const words& r) {
    const words s = mul_mod(words{{x, y, c}}, r);
    x = s[0];
    y = s[1];
    c = s[2];
  }

  /* 2^-(64 * 2^96) mod m: equivalent to 2^96 calls to next(). */
  static words get_jump() {
    return words{{0xd323a2a406f64957ULL, 0x21239e0fbd1ccbb5ULL, 0x702eb67f0959a76bULL}};
  }

  /* 2^-(64 * 2^144) mod m: equivalent to 2^144 calls to next(). */
  static words get_long_jump() {
    return words{{0x671d31e08a897fffULL, 0xbceb1c681d8e45f7ULL, 0xd6c731b43ecad305ULL}};
  }

public:
  inline static constexpr result_type min() {return 0;};
  inline static constexpr result_type max() {return UINT64_MAX;};

  mwc192x(result_type _seed = 0x85c6ea9eb065ebeeULL) {
    seed(_seed);
  }

  void seed(std::function<result_type(void)> rng) {
    x = rng();
    y = rng();
    // The value r used for c is read as a fixed-point fraction r / 2^64 in
    // [0..1) and multiplied by MWC_A2 - 2, the size of the range of valid
    // carries: c = 1 + (r * (MWC_A2 - 2) >> 64).
    uint64_t hi;
    mul_add(rng(), MWC_A2 - 2, 0, hi);
    c = 1 + hi;
  }

  void seed(result_type _seed) {
    seed(SplitMix(_seed));
  }

  result_type operator() () {
    const uint64_t result = y ^ c;
    uint64_t hi;
    const uint64_t lo = mul_add(MWC_A2, x, c, hi);
    x = y;
    y = lo;
    c = hi;
    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^96 calls to next(); it can be used to generate 2^95
   non-overlapping subsequences for parallel computations. jump(n)
   is equivalent to n calls to jump(), but it takes time logarithmic in n. */
  void jump() {
    do_jump(get_jump());
  }
  void jump(result_type n) {
    do_jump(pow_mod(get_jump(), n));
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^144 calls to next(); it can be used to generate 2^47 starting points,
   from each of which jump() will generate 2^48 non-overlapping
   subsequences for parallel distributed computations. long_jump(n)
   is equivalent to n calls to long_jump(), but it takes time logarithmic
   in n. */
  void long_jump() {
    do_jump(get_long_jump());
  }
  void long_jump(result_type n) {
    do_jump(pow_mod(get_long_jump(), n));
  }

  template< class CharT, class Traits >
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& ost, const mwc192x& e) {
    ost << e.x << ' ' << e.y << ' ' << e.c << ' ';
    return ost;
  }

  template< class CharT, class Traits >
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& ist, mwc192x& e) {
    ist >> e.x >> std::ws >> e.y >> std::ws >> e.c >> std::ws;
    return ist;
  }
};

}
#endif // MWC192X_H
