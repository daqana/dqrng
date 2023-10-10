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
#include <queue>
#include <Rcpp.h>
#include <dqrng_distribution.h>
#include <minimal_int_set.h>

namespace dqrng {
namespace sample {
template<typename VEC, typename INT>
inline VEC replacement(dqrng::random_64bit_generator &rng, INT n, INT size, int offset) {
  VEC result(size);
  std::generate(result.begin(), result.end(),
                [&n, &offset, &rng] () {return (offset + rng(n));});
  return result;
}

template<typename VEC, typename INT>
inline VEC fair_coin(dqrng::random_64bit_generator &rng, INT n, INT size, int head, int tail) {
  VEC result(size);
  INT k = 0;
  while (k < size) {
    uint64_t bits = rng();

    for (INT j = 0; j < 64 && k < size; ++k, ++j)
      result[k] = (bits >> j) & 1 ?  head : tail;
  }
  return result;
}


template<typename VEC, typename INT, typename FVEC>
inline VEC biased_coin(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, int head, int tail) {
  VEC result(size);

  // smaller probability scaled by 2^64 in order to compare directly with RNG output
  uint64_t p;
  if (prob[0] < 0.5)
    p = 0x1p64 * prob[0] / (prob[0] + prob[1]);
  else {
    p = 0x1p64 * prob[1] / (prob[0] + prob[1]);
    std::swap(head, tail);
  }
  std::generate(result.begin(), result.end(),
                [&p, &rng, &head, &tail] () {
                  return rng() < p ? head : tail;
                });
  return result;
}

template<typename VEC, typename INT, typename FVEC>
inline VEC replacement_prob(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, double max_prob, int offset) {
  VEC result(size);
  std::generate(result.begin(), result.end(),
                [&n, &prob, &max_prob, &rng, &offset] () {
                  while (true) {
                    INT index = rng(n);
                    if (dqrng::uniform01(rng()) < prob[index] / max_prob)
                      return index + offset;
                  }
                });
  return result;
}

template<typename VEC, typename INT, typename FVEC>
inline VEC replacement_alias(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, double prob_sum, int offset) {
  VEC result(size);
  std::vector<INT> alias(n);
  FVEC p(n);
  std::transform(prob.begin(), prob.end(), p.begin(),
                 [&n, &prob_sum] (double x) {return x * n / prob_sum;});
  std::queue<INT> high;
  std::queue<INT> low;
  for(INT i = 0; i < n; ++i) {
    if (p[i] < 1.0)
      low.push(i);
    else
      high.push(i);
  }
  while(!low.empty() && !high.empty()) {
    INT l = low.front();
    low.pop();
    INT h = high.front();
    alias[l] = h;
    p[h] = (p[h] + p[l]) - 1.0;
    if (p[h] < 1.0) {
      low.push(h);
      high.pop();
    }
  }
  while (!low.empty()) {
    p[low.front()] = 1.0;
    low.pop();
  }
  while (!high.empty()) {
    p[high.front()] = 1.0;
    high.pop();
  }
  std::generate(result.begin(), result.end(),
                [&n, &p, &alias, &rng, &offset] () {
                  INT index = rng(n);
                  if (dqrng::uniform01(rng()) < p[index])
                    return index + offset;
                  else
                    return alias[index] + offset;
                });

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

template<typename VEC, typename INT, typename FVEC>
inline VEC no_replacement_exp(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, int offset) {
  VEC index(n);
  std::iota(index.begin(), index.end(), (offset));
  FVEC weight(n);
  dqrng::exponential_distribution exponential;
  std::transform(prob.begin(), prob.end(), weight.begin(),
                 [&rng, &exponential] (double x) {return exponential(rng) / x;});
  std::partial_sort(index.begin(), index.begin() + size, index.end(),
                    [&weight](size_t i1, size_t i2) {return weight[i1] < weight[i2];});
  if (n == size)
    return index;
  else
    return VEC(index.begin(), index.begin() + size);
}

template<typename VEC, typename INT>
inline VEC sample(dqrng::random_64bit_generator &rng, INT n, INT size, bool replace, int offset = 0) {
  static_assert(std::is_integral<INT>::value && std::is_unsigned<INT>::value);
  static_assert(std::is_floating_point<typename VEC::value_type>::value ||
                std::is_integral<typename VEC::value_type>::value ||
                std::is_reference<typename VEC::value_type>::value);
  if (replace || size <= 1) {
    if (n == 2)
      return dqrng::sample::fair_coin<VEC, INT>(rng, n, size, offset, 1 + offset);

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

template<typename VEC, typename INT, typename FVEC>
inline VEC sample(dqrng::random_64bit_generator &rng, INT n, INT size, bool replace, FVEC prob, int offset = 0) {
  static_assert(std::is_integral<INT>::value && std::is_unsigned<INT>::value);
  static_assert(std::is_floating_point<typename VEC::value_type>::value ||
                std::is_integral<typename VEC::value_type>::value ||
                std::is_reference<typename VEC::value_type>::value);
  static_assert(std::is_floating_point<typename FVEC::value_type>::value ||
                std::is_reference<typename FVEC::value_type>::value);
  if (n != prob.size())
    Rcpp::stop("Argument requirements not fulfilled: n == prob.size()");
  if (replace || size <= 1) {
    if (n == 2)
      return dqrng::sample::biased_coin<VEC, INT>(rng, n, size, prob, offset, 1 + offset);

    double prob_sum = std::accumulate(prob.begin(), prob.end(), 0.0);
    if (size >= n)
      return dqrng::sample::replacement_alias<VEC, INT>(rng, n, size, prob, prob_sum, offset);

    double *max_prob = std::max_element(prob.begin(), prob.end());
    if (*max_prob * n / prob_sum < 3.)
      return dqrng::sample::replacement_prob<VEC, INT>(rng, n, size, prob, *max_prob, offset);
    else
      return dqrng::sample::replacement_alias<VEC, INT>(rng, n, size, prob, prob_sum, offset);
  } else {
    if (!(n >= size))
      Rcpp::stop("Argument requirements not fulfilled: n >= size");
    return dqrng::sample::no_replacement_exp<VEC, INT>(rng, n, size, prob, offset);
  }
}
} // sample
} // dqrng

#endif // DQRNG_SAMPLE_H
