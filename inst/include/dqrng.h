// Copyright 2023 Ralf Stubner
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

#ifndef dqrng_H
#define dqrng_H

#include "dqrng_RcppExports.h"

namespace dqrng {
inline random_64bit_accessor::random_64bit_accessor() : gen(dqrng::get_rng()) {}
} // namespace dqrng
#endif // dqrng_H
