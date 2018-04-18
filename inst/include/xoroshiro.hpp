/*
 MIT License

 Copyright (c) 2016 Alex Coplan
 Copyright (c) 2018 Ralf Stubner (daqana GmbH)

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 */

#ifndef AJC_HGUARD_XOROSHIRO128PLUS
#define AJC_HGUARD_XOROSHIRO128PLUS

#include <random>
#include <array>
#include <functional>

/* C++11 UniformRandomBitGenerator wrapper around xoroshift128+.
 * Original C code from: http://xoroshiro.di.unimi.it/xoroshiro128plus.c */

struct xoroshiro128plus_engine {
private:
  uint64_t state[2];

  static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
  }

  struct splitmix
  {
    splitmix(const uint64_t& k) : state(k) {}

    uint64_t next()
    {
      uint64_t z = (state += 0x9e3779b97f4a7c15ULL);
      z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
      z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
      return z ^ (z >> 31);
    }

  private:
    uint64_t state;
  };

public:
  using result_type = uint64_t;
  constexpr static result_type min() { return 0; }
  constexpr static result_type max() { return -1; }

  xoroshiro128plus_engine(result_type seed) {
    splitmix init(seed);
    state[0] = init.next();
    state[1] = init.next();
  }

  result_type operator()() {
      const uint64_t s0 = state[0];
      uint64_t s1 = state[1];
      const uint64_t result = s0 + s1;

      s1 ^= s0;
      state[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
      state[1] = rotl(s1, 36); // c

      return result;
    };

  void seed(std::function<uint32_t(void)> f) {
    uint64_t x_0 = f();
    uint64_t x_1 = f();
    state[0] = (x_0 << 32) | x_1;
    x_0 = f();
    x_1 = f();
    state[1] = (x_0 << 32) | x_1;
  };

  void seed(const std::array<uint32_t, 4> &a) {
    state[0] = ((uint64_t)a[0] << 32) | (uint64_t)a[1];
    state[1] = ((uint64_t)a[2] << 32) | (uint64_t)a[3];
  };

  void seed (result_type val = 0x28e12bd825206217ULL)
  {
    splitmix init(val);
    state[0] = init.next();
    state[1] = init.next();
  }

};

#endif
