// Copyright 2024 Ralf Stubner
// Copyright 2024 Philippe Grosjean
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

#include <sstream>
#include <dqrng.h>
#include <dqrng_distribution.h>
#include <RcppParallel/RVector.h>
#include <omp.h>

template<typename Dist, typename... Params>
Rcpp::NumericVector parallel_generate(int n, int threads, int streams, Params&&... params) {
  if (n < streams)
    streams = n;
  int stream_size = n/streams;
  int remainder = n % streams;
  Rcpp::NumericVector res(Rcpp::no_init(n));
  RcppParallel::RVector<double> work(res);

  // use global RNG from dqrng
  dqrng::random_64bit_accessor rng{};
  std::stringstream buffer;

#ifdef _OPENMP
  int maxthreads = omp_get_num_procs();
  if (threads > maxthreads)
    threads = maxthreads;
  // No need for more threads than there are streams
  if (threads > streams)
    threads = streams;
#endif

#pragma omp parallel num_threads(threads)
{
  int start,end;

#pragma omp for schedule(static,1)
  for (int i = 0; i < streams; ++i) {
    if (i < remainder) {
      start = i * stream_size + i;
      end = start + stream_size + 1;
    } else {
      start = i * stream_size + remainder;
      end = start + stream_size;
    }
    // Our private RNG in each stream; RNG with i == 0 is identical to global RNG
    auto prng = rng.clone(i);
    prng->generate<Dist>(std::begin(work) + start, std::begin(work) + end,
                         std::forward<Params>(params)...);
    if (i == 0) {// Save the state of the global RNG's clone
      buffer << *prng;
    }
  }
}
  // Make sure that the global RNG advances as well by applying the state of the global RNG's clone to the global RNG
  buffer >> rng;
  return res;
}
