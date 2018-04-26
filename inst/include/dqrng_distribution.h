// Copyright 2018 Ralf Stubner (daqana GmbH)
//
// This file is part of dqrng.
//
// dqrng is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// dqrng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
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
typedef boost::random::uniform_real_distribution<double> uniform_distribution;
// Boost's implementation uses the fast Ziggurat method
typedef boost::random::normal_distribution<double> normal_distribution;
typedef boost::random::exponential_distribution<double> exponential_distribution;

template<class DIST, class RES>
inline RES generate(size_t n, std::shared_ptr<random_64bit_generator> rng, const DIST& dist) {
  RES result(n);
  auto gen = std::bind(dist, std::ref(*rng.get()));
  std::generate(result.begin(), result.end(), gen);
  return result;
}

inline double uniform01(uint64_t x) {
  // prefer high bits due to weakness of lowest bits for xorshift/xoroshiro
  return (x >> 11) * (1. / (UINT64_C(1) << 53));
}
} // namespace dqrng

namespace boost {
namespace random {
namespace detail {

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(dqrng::random_64bit_generator& eng)
{
  dqrng::random_64bit_generator::result_type x = eng();
  double r = dqrng::uniform01(x);
  // shift x due to weakness of lowest bits for xorshift/xoroshiro
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
