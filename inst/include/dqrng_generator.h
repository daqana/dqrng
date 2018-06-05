// Copyright 2018 Ralf Stubner (daqana GmbH)
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

#include <cstdint>
#include <memory>
#include <random>
#include <type_traits>

namespace dqrng {
// conservative default
using default_64bit_generator = std::mt19937_64;

class random_64bit_generator {
public:
  typedef uint64_t result_type;

  virtual ~random_64bit_generator() {};
  virtual result_type operator() () = 0;
  virtual void seed(result_type seed) = 0;
  result_type min() {return 0.0;};
  result_type max() {return UINT64_MAX;};
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

public:
  random_64bit_wrapper() : gen() {};
  random_64bit_wrapper(result_type seed) : gen(seed) {};
  virtual result_type operator() () {return gen();}
  virtual void seed(result_type seed) {gen.seed(seed);}
};

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

template<typename RNG = default_64bit_generator>
rng64_t generator() {
  uint64_t seed = std::random_device{}();
  return generator<RNG>(seed);
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
