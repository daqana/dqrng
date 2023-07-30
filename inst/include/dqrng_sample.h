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
#include <dqrng_distribution.h>
#include <minimal_int_set.h>

namespace dqrng {
namespace sample {
template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> replacement(std::function<INT(INT)> selector, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> result(Rcpp::no_init(n));
  std::generate(result.begin(), result.end(),
                [m, offset, selector] () {return static_cast<storage_t>(offset + selector(m));});
  return result;
}

template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> no_replacement_shuffle(std::function<INT(INT)> selector, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> tmp(Rcpp::no_init(m));
  std::iota(tmp.begin(), tmp.end(), static_cast<storage_t>(offset));
  for (INT i = 0; i < n; ++i) {
    std::swap(tmp[i], tmp[i + selector(m - i)]);
  }
  if (m == n)
    return tmp;
  else
    return Rcpp::Vector<RTYPE>(tmp.begin(), tmp.begin() + n);
}

template<int RTYPE, typename INT, typename SET>
inline Rcpp::Vector<RTYPE> no_replacement_set(std::function<INT(INT)> selector, INT m, INT n, int offset) {
  using storage_t = typename Rcpp::traits::storage_type<RTYPE>::type;
  Rcpp::Vector<RTYPE> result(Rcpp::no_init(n));
  SET elems(m, n);
  for (INT i = 0; i < n; ++i) {
    INT v = selector(m);
    while (!elems.insert(v)) {
      v = selector(m);
    }
    result(i) = static_cast<storage_t>(offset + v);
  }
  return result;
}

template<typename INT>
inline INT roulette_wheel_selection(const dqrng::rng64_t &rng, INT m, Rcpp::NumericVector probs, double max_probs) {
  while (true) {
    INT index = (*rng)(m);
    if (dqrng::uniform01((*rng)()) < probs(index) / max_probs)
      return index;
  }
}

template<int RTYPE, typename INT>
inline Rcpp::Vector<RTYPE> sample(const dqrng::rng64_t &rng,
                                  INT m,
                                  INT n,
                                  bool replace,
                                  Rcpp::Nullable<Rcpp::NumericVector> probs = R_NilValue,
                                  int offset = 0) {
  std::function<INT(INT)> selector = [rng] (INT m) {return (*rng)(m);};
  if (probs.isNotNull()) {
    Rcpp::NumericVector tmp = probs.as();
    double max_probs = Rcpp::max(tmp);
    selector = [rng, tmp, max_probs] (INT m) {return roulette_wheel_selection<INT>(rng, m, tmp, max_probs);};
  }
  if (replace || n <= 1) {
    return dqrng::sample::replacement<RTYPE, INT>(selector, m, n, offset);
  } else {
    if (!(m >= n))
      Rcpp::stop("Argument requirements not fulfilled: m >= n");
    if (m < 2 * n) {
      return dqrng::sample::no_replacement_shuffle<RTYPE, INT>(selector, m, n, offset);
    } else if (m < 1000 * n) {
      return dqrng::sample::no_replacement_set<RTYPE, INT, dqrng::minimal_bit_set>(selector, m, n, offset);
    } else {
      return dqrng::sample::no_replacement_set<RTYPE, INT, dqrng::minimal_hash_set<INT>>(selector, m, n, offset);
    }
  }
}
} // sample
} // dqrng

#endif // DQRNG_SAMPLE_H
