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

#ifndef DQRNG_THREEFRY_H
#define DQRNG_THREEFRY_H 1

#include <sstream>
#include <vector>
#include <threefry.h>
#include <dqrng_generator.h>

namespace dqrng {

template<>
inline void random_64bit_wrapper<sitmo::threefry_20_64>::set_stream(result_type stream) {
  uint64_t number;
  std::vector<uint64_t> state;
  std::stringstream iss;
  iss << gen;
  while (iss >> number)
    state.push_back(number);
  // state[4:7] is the current counter, the highest part is incremented by stream
  gen.set_counter(state[4], state[5], state[6], state[7] + stream);
}
} // namespace dqrng

#endif // DQRNG_THREEFRY_H
