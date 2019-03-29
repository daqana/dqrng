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

#ifndef DQRNG_DISTRIBUTION_H
#define DQRNG_DISTRIBUTION_H 1

#include <cstdint>
#include <memory>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <dqrng_generator.h>

namespace dqrng {
// Boost's implementation allow for performance increasing spezializations
using uniform_distribution = boost::random::uniform_real_distribution<double>;
// Boost's implementation uses the fast Ziggurat method
using normal_distribution = boost::random::normal_distribution<double>;
using exponential_distribution = boost::random::exponential_distribution<double>;

template<typename DIST, typename RES>
inline RES generate(size_t n, rng64_t rng, const DIST& dist) {
  RES result(n);
  auto gen = std::bind(dist, std::ref(*rng));
  std::generate(result.begin(), result.end(), gen);
  return result;
}

inline double uniform01(uint64_t x) {
  // prefer high bits due to weakness of lowest bits for xoshiro/xoroshiro with used "+" scrambler
  return (x >> 11) * (1. / (UINT64_C(1) << 53));
}


inline uint32_t random32(dqrng::random_64bit_generator& rng) {
    static bool has_cache{false};
    static uint32_t cache;
    if (has_cache) {
        has_cache = false;
        return cache;
    }
    uint64_t random = rng();
    cache = uint32_t(random);
    has_cache = true;
    return random >> 32;
}

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

inline uint32_t bounded_rand32(dqrng::random_64bit_generator& rng, uint32_t range) {
    uint32_t x = random32(rng);
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
            x = random32(rng);
            m = uint64_t(x) * uint64_t(range);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

#ifdef LONG_VECTOR_SUPPORT
inline uint64_t bounded_rand64(dqrng::random_64bit_generator& rng, uint64_t range) {
    using pcg_extras::pcg128_t;
    uint64_t x = rng();
    pcg128_t m = pcg128_t(x) * pcg128_t(range);
    uint64_t l = uint64_t(m);
    if (l < range) {
        uint64_t t = -range;
        if (t >= range) {
            t -= range;
            if (t >= range)
                t %= range;
        }
        while (l < t) {
            x = rng();
            m = pcg128_t(x) * pcg128_t(range);
            l = uint64_t(m);
        }
    }
    return m >> 64;
}
#endif
} // namespace dqrng

namespace boost {
namespace random {
namespace detail {

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(dqrng::random_64bit_generator& eng)
{
  dqrng::random_64bit_generator::result_type x = eng();
  double r = dqrng::uniform01(x);
  // shift x due to weakness of lowest bits for xoshiro/xoroshiro with used "+" scrambler
  int bucket = (x >> 3) & 0xFF;
  return std::make_pair(r, bucket);
}

template<>
inline double generate_uniform_real<dqrng::random_64bit_generator, double>(dqrng::random_64bit_generator& eng, double min, double max)
{
  return dqrng::uniform01(eng()) * (max - min) + min;
}

} // namespace detail
} // namespace random
} // namespace boost

#endif // DQRNG_DISTRIBUTION_H
