// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2023-2024 Ralf Stubner
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

#include <mystdint.h>
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

inline double uniform01(uint64_t x) {
  // prefer high bits due to weakness of lowest bits for xoshiro/xoroshiro with the "+" scrambler
  return (x >> 11) * 0x1.0p-53;
}
} // namespace dqrng

namespace boost {
namespace random {
namespace detail {

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(dqrng::random_64bit_generator& eng)
{
  return eng.generate_double_8bit_pair();
}

template<>
inline double generate_uniform_real<dqrng::random_64bit_generator, double>(dqrng::random_64bit_generator& eng, double min, double max)
{
  return eng.uniform01() * (max - min) + min;
}

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_accessor>(dqrng::random_64bit_accessor& eng)
{
  return generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(eng);
}

template<>
inline double generate_uniform_real<dqrng::random_64bit_accessor, double>(dqrng::random_64bit_accessor& eng, double min, double max)
{
  return generate_uniform_real<dqrng::random_64bit_generator, double>(eng, min, max);
}

} // namespace detail
} // namespace random
} // namespace boost

#endif // DQRNG_DISTRIBUTION_H
