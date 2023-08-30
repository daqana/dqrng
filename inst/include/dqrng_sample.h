// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2022 Ralf Stubner
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
#include <dqrng_generator.h>
#include <minimal_int_set.h>

namespace dqrng {
namespace sample {
template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> replacement(dqrng::rng64_t &rng, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> result(Rcpp::no_init(n));
  std::generate(result.begin(), result.end(),
                [m, offset, rng] () {return static_cast<storage_t>(offset + (*rng)(m));});
  return result;
}

template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> no_replacement_shuffle(dqrng::rng64_t &rng, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> tmp(Rcpp::no_init(m));
  std::iota(tmp.begin(), tmp.end(), static_cast<storage_t>(offset));
  for (INT i = 0; i < n; ++i) {
    std::swap(tmp[i], tmp[i + (*rng)(m - i)]);
  }
  if (m == n)
    return tmp;
  else
    return Rcpp::Vector<RTYPE>(tmp.begin(), tmp.begin() + n);
}

template<int RTYPE, typename INT, typename SET>
inline Rcpp::Vector<RTYPE> no_replacement_set(dqrng::rng64_t &rng, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> result(Rcpp::no_init(n));
  SET elems(m, n);
  for (INT i = 0; i < n; ++i) {
    INT v = (*rng)(m);
    while (!elems.insert(v)) {
      v = (*rng)(m);
    }
    result(i) = static_cast<storage_t>(offset + v);
  }
  return result;
}

template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> sample(dqrng::rng64_t &rng, INT m, INT n, bool replace, int offset) {
  if (replace || n <= 1) {
    return dqrng::sample::replacement<RTYPE, INT>(rng, m, n, offset);
  } else {
    if (!(m >= n))
      Rcpp::stop("Argument requirements not fulfilled: m >= n");
    if (m < 2 * n) {
      return dqrng::sample::no_replacement_shuffle<RTYPE, INT>(rng, m, n, offset);
    } else if (m < 1000 * n) {
      return dqrng::sample::no_replacement_set<RTYPE, INT, dqrng::minimal_bit_set>(rng, m, n, offset);
    } else {
      return dqrng::sample::no_replacement_set<RTYPE, INT, dqrng::minimal_hash_set<INT>>(rng, m, n, offset);
    }
  }
}
} // sample
} // dqrng

#endif // DQRNG_SAMPLE_H
