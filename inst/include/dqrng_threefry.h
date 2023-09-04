// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2023 Ralf Stubner
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

#include <threefry.h>
#include <dqrng_generator.h>

namespace dqrng {

template<>
inline void random_64bit_wrapper<sitmo::threefry_20_64>::seed(result_type seed, result_type stream) {
  gen.seed(seed);
  gen.set_counter(0, 0, 0, stream);
  cache = false;
}
} // namespace dqrng

#endif // DQRNG_THREEFRY_H
