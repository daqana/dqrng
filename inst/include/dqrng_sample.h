// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2022-2023 Ralf Stubner
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

#ifndef DQRNG_SAMPLE_H
#define DQRNG_SAMPLE_H 1

#include <mystdint.h>
#include <Rcpp.h>
#include <dqrng_types.h>
#include <minimal_int_set.h>

namespace dqrng {
namespace sample {
template<typename VEC, typename INT>
inline VEC replacement(dqrng::random_64bit_generator &rng, INT n, INT size, int offset) {
  VEC result(size);
  std::generate(result.begin(), result.end(),
                [n, offset, &rng] () {return (offset + rng(n));});
  return result;
}

template<typename VEC, typename INT>
inline VEC no_replacement_shuffle(dqrng::random_64bit_generator &rng, INT n, INT size, int offset) {
  VEC tmp(n);
  std::iota(tmp.begin(), tmp.end(), (offset));
  for (INT i = 0; i < size; ++i) {
    std::swap(tmp[i], tmp[i + rng(n - i)]);
  }
  if (n == size)
    return tmp;
  else
    return VEC(tmp.begin(), tmp.begin() + size);
}

template<typename VEC, typename INT, typename SET>
inline VEC no_replacement_set(dqrng::random_64bit_generator &rng, INT n, INT size, int offset) {
  VEC result(size);
  SET elems(n, size);
  for (INT i = 0; i < size; ++i) {
    INT v;
    do {
      v = rng(n);
    } while (!elems.insert(v));
    result[i] = (offset + v);
  }
  return result;
}

template<typename VEC, typename INT>
inline VEC sample(dqrng::random_64bit_generator &rng, INT n, INT size, bool replace, int offset = 0) {
  if (replace || size <= 1) {
    return dqrng::sample::replacement<VEC, INT>(rng, n, size, offset);
  } else {
    if (!(n >= size))
      Rcpp::stop("Argument requirements not fulfilled: n >= size");
    if (n < 2 * size) {
      return dqrng::sample::no_replacement_shuffle<VEC, INT>(rng, n, size, offset);
    } else if (n < 1000 * size) {
      return dqrng::sample::no_replacement_set<VEC, INT, dqrng::minimal_bit_set>(rng, n, size, offset);
    } else {
      return dqrng::sample::no_replacement_set<VEC, INT, dqrng::minimal_hash_set<INT>>(rng, n, size, offset);
    }
  }
}
} // sample
} // dqrng

#endif // DQRNG_SAMPLE_H
