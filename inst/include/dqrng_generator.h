// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
//
// This file is part of dqrng.
//
// dqrng is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dqrng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with dqrng.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DQRNG_GENERATOR_H
#define DQRNG_GENERATOR_H 1

#include <mystdint.h>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <xoshiro.h>
#include <pcg_random.hpp>
#include <threefry.h>

namespace dqrng {
using default_64bit_generator = ::dqrng::xoroshiro128plus;

class random_64bit_generator {
public:
  using result_type = uint64_t;

  virtual ~random_64bit_generator() {};
  virtual result_type operator() () = 0;
  virtual void seed(result_type seed) = 0;
  virtual void seed(result_type seed, result_type stream) = 0;
  static constexpr result_type min() {return 0;};
  static constexpr result_type max() {return UINT64_MAX;};
  virtual uint32_t operator() (uint32_t range) = 0;
#ifdef LONG_VECTOR_SUPPORT
  virtual uint64_t operator() (uint64_t range) = 0;
#endif
};

using rng64_t = std::shared_ptr<random_64bit_generator>;

template<typename RNG>
class random_64bit_wrapper : public random_64bit_generator {
  static_assert(std::is_same<random_64bit_generator::result_type, typename RNG::result_type>::value,
                "Provided RNG has wrong result_type");
  static_assert(RNG::max() == UINT64_MAX, "Provided RNG has wrong maximum.");
  static_assert(RNG::min() == 0, "Provided RNG has wrong minimum.");
private:
  RNG gen;
  bool has_cache{false};
  uint32_t cache;

  uint64_t bit64() {return gen();}
  uint32_t bit32() {
    if (has_cache) {
      has_cache = false;
      return cache;
    }
    uint64_t random = this->bit64();
    cache = uint32_t(random);
    has_cache = true;
    return random >> 32;
  }

public:
  random_64bit_wrapper() : gen() {};
  random_64bit_wrapper(result_type seed) : gen(seed) {};
  virtual result_type operator() () {return this->bit64();}
  virtual void seed(result_type seed) {cache = false; gen.seed(seed);}
  virtual void seed(result_type seed, result_type stream) {throw std::runtime_error("Stream handling not supported for this RNG!");}

  /*
   * https://raw.githubusercontent.com/imneme/bounded-rands/3d71f53c975b1e5b29f2f3b05a74e26dab9c3d84/bounded32.cpp
   * https://raw.githubusercontent.com/imneme/bounded-rands/3d71f53c975b1e5b29f2f3b05a74e26dab9c3d84/bounded64.cpp
   * A C++ implementation methods and benchmarks for random numbers in a range
   * (64 and 32-bit version)
   *
   * The MIT License (MIT)
   *
   * Copyright (c) 2018 Melissa E. O'Neill
   *
   * Permission is hereby granted, free of charge, to any person obtaining a
   * copy of this software and associated documentation files (the "Software"),
   * to deal in the Software without restriction, including without limitation
   * the rights to use, copy, modify, merge, publish, distribute, sublicense,
   * and/or sell copies of the Software, and to permit persons to whom the
   * Software is furnished to do so, subject to the following conditions:
   *
   * The above copyright notice and this permission notice shall be included in
   * all copies or substantial portions of the Software.
   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   * DEALINGS IN THE SOFTWARE.
   */

  virtual uint32_t operator() (uint32_t range) {
    uint32_t x = this->bit32();
    uint64_t m = uint64_t(x) * uint64_t(range);
    uint32_t l = uint32_t(m);
    if (l < range) {
      uint32_t t = -range;
      if (t >= range) {
        t -= range;
        if (t >= range)
          t %= range;
      }
      while (l < t) {
        x = this->bit32();
        m = uint64_t(x) * uint64_t(range);
        l = uint32_t(m);
      }
    }
    return m >> 32;
  }

#ifdef LONG_VECTOR_SUPPORT
  virtual uint64_t operator() (uint64_t range) {
    using pcg_extras::pcg128_t;
    uint64_t x = this->bit64();
    pcg128_t m = pcg128_t(x) * pcg128_t(range);
    uint64_t l = uint64_t(m);
    if (l < range) { // # nocov start
      uint64_t t = -range;
      if (t >= range) {
        t -= range;
        if (t >= range)
          t %= range;
      }
      while (l < t) {
        x = this->bit64();
        m = pcg128_t(x) * pcg128_t(range);
        l = uint64_t(m);
      }
    } // # nocov end
    return m >> 64;
  }
#endif
};

template<>
inline void random_64bit_wrapper<::dqrng::xoroshiro128plus>::seed(result_type seed, result_type stream) {
    gen.seed(seed);
    gen.jump(stream);
    cache = false;
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256plus>::seed(result_type seed, result_type stream) {
    gen.seed(seed);
    gen.long_jump(stream);
    cache = false;
}

template<>
inline void random_64bit_wrapper<pcg64>::seed(result_type seed, result_type stream) {
    gen.seed(seed, stream);
    cache = false;
}

template<>
inline void random_64bit_wrapper<sitmo::threefry_20_64>::seed(result_type seed, result_type stream) {
    gen.seed(seed);
    gen.set_counter(0, 0, 0, stream);
    cache = false;
}


template<typename RNG = default_64bit_generator>
typename std::enable_if<!std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed) {
  return std::make_shared<random_64bit_wrapper<RNG>>(seed);
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed) {
  return std::make_shared<RNG>(seed);
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
