// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2023-2024 Ralf Stubner
// Copyright 2023 Henrik Sloot
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

#ifndef DQRNG_TYPES_H
#define DQRNG_TYPES_H 1

#include <mystdint.h>
#include <stdexcept>
#include <memory>
#include <Rcpp/XPtr.h>
#include <pcg_extras.hpp>

namespace dqrng {

class random_64bit_generator {
private:
  uint64_t bit64() {return this->operator()();}
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

protected:
  bool has_cache{false};
  uint32_t cache;

  virtual void output(std::ostream& ost) const = 0;
  virtual void input(std::istream& ist) = 0;

public:
  using result_type = uint64_t;

  virtual ~random_64bit_generator() {};
  virtual result_type operator() () = 0;
  virtual void seed(result_type seed) = 0;
  virtual void seed(result_type seed, result_type stream) = 0;
  virtual std::unique_ptr<random_64bit_generator> clone(result_type stream) = 0;
  static constexpr result_type min() {return 0;};
  static constexpr result_type max() {return UINT64_MAX;};
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

  uint32_t operator() (uint32_t range) {
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
  uint64_t operator() (uint64_t range) {
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
  friend std::ostream& operator<<(std::ostream& ost, const random_64bit_generator& e) {
    e.output(ost);
    return ost;
  }
  friend std::istream& operator>>(std::istream& ist, random_64bit_generator& e) {
    e.input(ist);
    return ist;
  }
};

class random_64bit_accessor : public random_64bit_generator {
private:
  Rcpp::XPtr<dqrng::random_64bit_generator> gen;

protected:
  virtual void output(std::ostream& ost) const override {
    ost << *gen;
  }

  virtual void input(std::istream& ist) override {
    ist >> *gen;
  }

public:
  explicit random_64bit_accessor();
  // defined in dqrng.h:
  // random_64bit_accessor() : gen(dqrng::get_rng()) {}

  virtual result_type operator() () override {
    return (*gen)();
  };

  virtual void seed(result_type seed) override {
    throw std::runtime_error("Seed handling not supported for this class!");
  };

  virtual void seed(result_type seed, result_type stream) override {
    throw std::runtime_error("Seed handling not supported for this class!");
  };

  virtual std::unique_ptr<random_64bit_generator> clone(result_type stream) override {
    return gen->clone(stream);
  };
};

} // namespace dqrng
#endif // DQRNG_TYPES_H
