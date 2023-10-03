/*
 Original C Code written in 2016-2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)
 Adapted to C++: 2018-2019 by Ralf Stubner (daqana GmbH)
                 2023 by Ralf Stubner

 To the extent possible under law, the author has dedicated all copyright
 and related and neighboring rights to this software to the public domain
 worldwide. This software is distributed without any warranty.

 See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#ifndef XOSHIRO_H
#define XOSHIRO_H 1

#include <array>
#include <mystdint.h>
#include <functional>
#include <algorithm>

namespace dqrng {
template<size_t N>
class xoshiro {
public:
  using result_type = uint64_t;

  virtual std::array<result_type, N> get_jump() = 0;
  virtual std::array<result_type, N> get_long_jump() = 0;
  void do_jump(std::array<result_type, N>);

protected:
  std::array<result_type, N> s;

  result_type rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
  }

private:
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

  virtual result_type next() = 0;

public:
  inline static constexpr result_type min() {return 0.0;};
  inline static constexpr result_type max() {return UINT64_MAX;};

  xoshiro(result_type _seed = 0x85c6ea9eb065ebeeULL) {
    seed(_seed);
  }

  virtual void seed(std::function<result_type(void)> rng) {
    std::generate(s.begin(), s.end(), rng);
  }

  virtual void seed(result_type _seed) {
    seed(SplitMix(_seed));
  }

  virtual result_type operator() () {
    return next();
  }

  void jump() {
    do_jump(get_jump());
  };
  void jump(result_type n) {
    for( ; n > 0; --n) jump();
  }
  void long_jump() {
    do_jump(get_long_jump());
  };
  void long_jump(result_type n) {
    for( ; n > 0; --n) long_jump();
  }

  template< class CharT, class Traits >
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& ost, const xoshiro<N>& e) {
    for(unsigned int i = 0; i < N; ++i) {
      ost << e.s[i] << ' ';
    }
    return ost;
  }

  template< class CharT, class Traits >
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& ist, xoshiro<N>& e) {
    for(unsigned int i = 0; i < N; ++i) {
      ist >> e.s[i] >> std::ws;
    }
    return ist;
  }
};

template<>
void xoshiro<2>::do_jump(std::array<result_type, 2> JUMP) {
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    for(unsigned int i = 0; i < sizeof JUMP / sizeof JUMP.begin(); i++)
      for(int b = 0; b < 64; b++) {
        if (JUMP[i] & UINT64_C(1) << b) {
          s0 ^= s[0];
          s1 ^= s[1];
        }
        next();
      }

    s[0] = s0;
    s[1] = s1;
}

template<>
void xoshiro<4>::do_jump(std::array<result_type, 4> JUMP) {
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    for(unsigned int i = 0; i < sizeof JUMP / sizeof JUMP.begin(); i++)
      for(int b = 0; b < 64; b++) {
        if (JUMP[i] & UINT64_C(1) << b) {
          s0 ^= s[0];
          s1 ^= s[1];
          s2 ^= s[2];
          s3 ^= s[3];
        }
        next();
      }

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;
}

/* This is xoroshiro128+ 1.0, our best and fastest small-state generator
   for floating-point numbers, but its state space is large enough only
   for mild parallelism. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than
   xoroshiro128++/xoroshiro128**. It passes all tests we are aware of
   except for the four lower bits, which might fail linearity tests (and
   just those), so if low linear complexity is not considered an issue (as
   it is usually the case) it can be used to generate 64-bit outputs, too;
   moreover, this generator has a very mild Hamming-weight dependency
   making our test (http://prng.di.unimi.it/hwd.php) fail after 5 TB of
   output; we believe this slight bias cannot affect any application. If
   you are concerned, use xoroshiro128++, xoroshiro128** or xoshiro256+.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s.

   NOTE: the parameters (a=24, b=16, b=37) of this version give slightly
   better results in our test than the 2016 version (a=55, b=14, c=36).
*/

class xoroshiro128plus : public xoshiro<2> {
public:
  xoroshiro128plus() : xoshiro() {};
  xoroshiro128plus(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = rotl(s1, 37); // c

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 2> get_jump() override {
    return std::array<result_type, 2>{0xdf900294d8f554a5, 0x170865df4b3201fc};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */
   std::array<result_type, 2> get_long_jump() override {
     return std::array<result_type, 2>{0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1};
   }
};

/* This is xoroshiro128** 1.0, one of our all-purpose, rock-solid,
 small-state generators. It is extremely (sub-ns) fast and it passes all
 tests we are aware of, but its state space is large enough only for
 mild parallelism.

 For generating just floating-point numbers, xoroshiro128+ is even
 faster (but it has a very mild bias, see notes in the comments).

 The state must be seeded so that it is not everywhere zero. If you have
 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
 output to fill s. */


class xoroshiro128starstar : public xoshiro<2> {
public:
  xoroshiro128starstar() : xoshiro() {};
  xoroshiro128starstar(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = rotl(s0 * 5, 7) * 9;

    s1 ^= s0;
    s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = rotl(s1, 37); // c

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 2> get_jump() override {
    return std::array<result_type, 2>{0xdf900294d8f554a5, 0x170865df4b3201fc};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */
  std::array<result_type, 2> get_long_jump() override {
    return std::array<result_type, 2>{0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1};
  }
};

/* This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid,
 small-state generators. It is extremely (sub-ns) fast and it passes all
 tests we are aware of, but its state space is large enough only for
 mild parallelism.

 For generating just floating-point numbers, xoroshiro128+ is even
 faster (but it has a very mild bias, see notes in the comments).

 The state must be seeded so that it is not everywhere zero. If you have
 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
 output to fill s. */


class xoroshiro128plusplus : public xoshiro<2> {
public:
  xoroshiro128plusplus() : xoshiro() {};
  xoroshiro128plusplus(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t s0 = s[0];
    uint64_t s1 = s[1];
    const uint64_t result = rotl(s0 + s1, 17) + s0;

    s1 ^= s0;
    s[0] = rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
    s[1] = rotl(s1, 28); // c

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 2> get_jump() override {
    return std::array<result_type, 2>{0x2bd7a6a6e99c2ddc, 0x0992ccaf6a6fca05};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */
  std::array<result_type, 2> get_long_jump() override {
    return std::array<result_type, 2>{0x360fd5f2cf8d5d99, 0x9c6e6877736c46e3};
  }
};

/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
 numbers. We suggest to use its upper bits for floating-point
 generation, as it is slightly faster than xoshiro256++/xoshiro256**. It
 passes all tests we are aware of except for the lowest three bits,
 which might fail linearity tests (and just those), so if low linear
 complexity is not considered an issue (as it is usually the case) it
 can be used to generate 64-bit outputs, too.

 We suggest to use a sign test to extract a random Boolean value, and
 right shifts to extract subsets of bits.

 The state must be seeded so that it is not everywhere zero. If you have
 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
 output to fill s. */

class xoshiro256plus  : public xoshiro<4> {
public:
  xoshiro256plus() : xoshiro() {};
  xoshiro256plus(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t result = s[0] + s[3];

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 4> get_jump() override {
    return std::array<result_type, 4>{0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */
  std::array<result_type, 4> get_long_jump() override {
    return std::array<result_type, 4>{0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635};
  }
};

/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
 generators. It has excellent (sub-ns) speed, a state (256 bits) that is
 large enough for any parallel application, and it passes all tests we
 are aware of.

 For generating just floating-point numbers, xoshiro256+ is even faster.

 The state must be seeded so that it is not everywhere zero. If you have
 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
 output to fill s. */

class xoshiro256starstar  : public xoshiro<4> {
public:
  xoshiro256starstar() : xoshiro() {};
  xoshiro256starstar(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t result = rotl(s[1] * 5, 7) * 9;

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 4> get_jump() override {
    return std::array<result_type, 4>{0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */
  std::array<result_type, 4> get_long_jump() override {
    return std::array<result_type, 4>{0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635};
  }
};



/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
 It has excellent (sub-ns) speed, a state (256 bits) that is large
 enough for any parallel application, and it passes all tests we are
 aware of.

 For generating just floating-point numbers, xoshiro256+ is even faster.

 The state must be seeded so that it is not everywhere zero. If you have
 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
 output to fill s. */

class xoshiro256plusplus  : public xoshiro<4> {
public:
  xoshiro256plusplus() : xoshiro() {};
  xoshiro256plusplus(result_type _seed) : xoshiro(_seed) {};

private:
  uint64_t next(void) override {
    const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

    const uint64_t t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
  }

  /* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */
  std::array<result_type, 4> get_jump() override {
    return std::array<result_type, 4>{0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c};
  }

  /* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */
  std::array<result_type, 4> get_long_jump() override {
    return std::array<result_type, 4>{0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635};
  }
};

}
#endif // XOSHIRO_H
