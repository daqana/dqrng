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

// stochastic acceptance
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

// create table for alias method (Walker/Voss)
template<typename INT, typename FVEC>
inline std::vector<std::pair<double,INT>> create_alias_table(INT n, FVEC prob, double prob_sum) {
  std::vector<std::pair<double,INT>> prob_alias(n);
  std::queue<INT> high;
  std::queue<INT> low;
  for(INT i = 0; i < n; ++i) {
    prob_alias[i].first = prob[i] * n / prob_sum;
    if (prob_alias[i].first < 1.0)
      low.push(i);
    else
      high.push(i);
  }
  while(!low.empty() && !high.empty()) {
    INT l = low.front();
    low.pop();
    INT h = high.front();
    prob_alias[l].second = h;
    prob_alias[h].first = (prob_alias[h].first + prob_alias[l].first) - 1.0;
    if (prob_alias[h].first < 1.0) {
      low.push(h);
      high.pop();
    }
  }
  while (!low.empty()) {
    prob_alias[low.front()].first = 1.0;
    low.pop();
  }
  while (!high.empty()) {
    prob_alias[high.front()].first = 1.0;
    high.pop();
  }
  return prob_alias;
}

// alias method (Walker/Voss)
template<typename VEC, typename INT, typename FVEC>
inline VEC replacement_alias(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, double prob_sum, int offset) {
  VEC result(size);
  std::vector<std::pair<double,INT>> prob_alias = create_alias_table(n, prob, prob_sum);
  std::generate(result.begin(), result.end(),
                [&n, &prob_alias, &rng, &offset] () {
                  INT index = rng(n);
                  return (dqrng::uniform01(rng()) < prob_alias[index].first) ?
                          index + offset : prob_alias[index].second + offset;
                });

    return result;
}

// Fisher-Yates shuffle
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

// set-based rejection sampling
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

// exponential rank (Efraimidis/Spirakis)
template<typename VEC, typename INT, typename FVEC>
inline VEC no_replacement_exp(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, int offset) {
  VEC index(n);
  std::iota(index.begin(), index.end(), 0);
  FVEC weight(n);
  dqrng::exponential_distribution exponential;
  std::transform(prob.begin(), prob.end(), weight.begin(),
                 [&rng, &exponential] (double x) {return exponential(rng) / x;});
  std::partial_sort(index.begin(), index.begin() + size, index.end(),
                    [&weight](size_t i1, size_t i2) {return weight[i1] < weight[i2];});

  VEC result(size);
  std::transform(index.begin(), index.begin() + size, result.begin(),
                 [&offset] (INT x) {return x + offset;});
  return result;
}

// set-based rejection sampling with stochastic acceptance
template<typename VEC, typename INT, typename SET, typename FVEC>
inline VEC no_replacement_prob_set(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, double max_prob, int offset) {
  VEC result(size);
  SET elems(n, size);

  for (INT i = 0; i < size; ++i) {
    INT v;
    do {
      do {
        v = rng(n);
      } while (dqrng::uniform01(rng()) >= prob[v] / max_prob);
    } while (!elems.insert(v));
    result[i] = (offset + v);
  }
  return result;
}

// set-based rejection sampling with alias selection
template<typename VEC, typename INT, typename SET, typename FVEC>
inline VEC no_replacement_alias_set(dqrng::random_64bit_generator &rng, INT n, INT size, FVEC prob, double prob_sum, int offset) {
  VEC result(size);
  std::vector<std::pair<double,INT>> prob_alias = create_alias_table(n, prob, prob_sum);
  SET elems(n, size);
  for (INT i = 0; i < size; ++i) {
    INT v;
    do {
      INT index = rng(n);
      v = (dqrng::uniform01(rng()) < prob_alias[index].first) ?
                            index : prob_alias[index].second;
    } while (!elems.insert(v));
    result[i] = (offset + v);
  }
  return result;
}

template<typename VEC, typename INT>
inline VEC sample(dqrng::random_64bit_generator &rng, INT n, INT size, bool replace, int offset = 0) {
  static_assert(std::is_integral<INT>::value && std::is_unsigned<INT>::value,
                "Provided INT has the wrong type.");
  static_assert(std::is_floating_point<typename VEC::value_type>::value ||
                std::is_integral<typename VEC::value_type>::value ||
                std::is_reference<typename VEC::value_type>::value,
                "Provided VEC has the wrong type.");
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
  static_assert(std::is_integral<INT>::value && std::is_unsigned<INT>::value,
                "Provided INT has the wrong type.");
  static_assert(std::is_floating_point<typename VEC::value_type>::value ||
                std::is_integral<typename VEC::value_type>::value ||
                std::is_reference<typename VEC::value_type>::value,
                "Provided VEC has the wrong type.");
  static_assert(std::is_floating_point<typename FVEC::value_type>::value ||
                std::is_reference<typename FVEC::value_type>::value,
                "Provided FVEC has the wrong type.");
  if (n != INT(prob.size()))
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
    if (n < 2 * size)
      return dqrng::sample::no_replacement_exp<VEC, INT>(rng, n, size, prob, offset);

    double prob_sum = std::accumulate(prob.begin(), prob.end(), 0.0);
    double *max_prob = std::max_element(prob.begin(), prob.end());
    if (n < 1000 * size) {// check this factor
      using set_t = dqrng::minimal_bit_set;
      if (*max_prob * n / prob_sum < 3.)
        return dqrng::sample::no_replacement_prob_set<VEC, INT, set_t>(rng, n, size, prob, *max_prob, offset);
      else
        return dqrng::sample::no_replacement_alias_set<VEC, INT, set_t>(rng, n, size, prob, prob_sum, offset);
    } else {
      using set_t = dqrng::minimal_hash_set<INT>;
      if (*max_prob * n / prob_sum < 3.)
        return dqrng::sample::no_replacement_prob_set<VEC, INT, set_t>(rng, n, size, prob, *max_prob, offset);
      else
        return dqrng::sample::no_replacement_alias_set<VEC, INT, set_t>(rng, n, size, prob, prob_sum, offset);
    }
  }
}
} // sample
} // dqrng

#endif // DQRNG_SAMPLE_H
