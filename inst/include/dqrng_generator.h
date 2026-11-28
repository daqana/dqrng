// Copyright 2018-2019 Ralf Stubner (daqana GmbH)
// Copyright 2023-2026 Ralf Stubner
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

#include <type_traits>
#include <stdexcept>
#include <dqrng_types.h>
#include <xoshiro.h>
#include <mwc192x.h>
#include <pcg_random.hpp>
#include <Rcpp.h>
#include <convert_seed.h>
#include <R_randgen.h>

#if defined(__cpp_lib_make_unique) && (__cpp_lib_make_unique >= 201304)
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

namespace dqrng {
using rng64_t = Rcpp::XPtr<random_64bit_generator>;
using default_64bit_generator = ::dqrng::xoroshiro128plusplus;

template<typename RNG>
class random_64bit_wrapper : public random_64bit_generator {
  static_assert(std::is_same<random_64bit_generator::result_type, typename RNG::result_type>::value,
                "Provided RNG has wrong result_type");
  static_assert(RNG::max() == UINT64_MAX, "Provided RNG has wrong maximum.");
  static_assert(RNG::min() == 0, "Provided RNG has wrong minimum.");
private:
  RNG gen;

protected:
  virtual void output(std::ostream& ost) const override {ost << gen;}
  virtual void input(std::istream& ist) override {ist >> gen;}
  void advance_stream(result_type stream) override {throw std::runtime_error("Stream handling not supported for this RNG!");}
  void advance_substream(result_type substream) override {throw std::runtime_error("Substream handling not supported for this RNG!");}

public:
  random_64bit_wrapper() : gen() {};
  random_64bit_wrapper(RNG _gen) : gen(_gen) {};
  random_64bit_wrapper(result_type seed) : gen(seed) {};
  random_64bit_wrapper(result_type seed, result_type stream) : gen() {this->seed(seed, stream);};
  virtual result_type operator() () override {return gen();}
  virtual void seed(result_type seed) override {cache = false; gen.seed(seed);}
  virtual void seed(result_type seed, result_type stream) override {cache = false; gen.seed(seed); this->advance_stream(stream);}
  virtual std::unique_ptr<random_64bit_generator> clone(result_type stream, bool use_substream = false) override {
    auto rng = make_unique<random_64bit_wrapper<RNG>>(gen);
    if (use_substream)
      rng->advance_substream(stream);
    else
      rng->advance_stream(stream);
    return rng;
  }
};

template<>
inline void random_64bit_wrapper<::dqrng::xoroshiro128plus>::advance_stream(result_type stream) {
  gen.jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoroshiro128plusplus>::advance_stream(result_type stream) {
  gen.jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoroshiro128starstar>::advance_stream(result_type stream) {
  gen.jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256plus>::advance_stream(result_type stream) {
  gen.long_jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256plusplus>::advance_stream(result_type stream) {
  gen.long_jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256starstar>::advance_stream(result_type stream) {
  gen.long_jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::mwc192x>::advance_stream(result_type stream) {
  gen.long_jump(stream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256plus>::advance_substream(result_type substream) {
  gen.jump(substream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256plusplus>::advance_substream(result_type substream) {
  gen.jump(substream);
}

template<>
inline void random_64bit_wrapper<::dqrng::xoshiro256starstar>::advance_substream(result_type substream) {
  gen.jump(substream);
}

template<>
inline void random_64bit_wrapper<::dqrng::mwc192x>::advance_substream(result_type substream) {
  gen.jump(substream);
}

#if !(defined(__APPLE__) && defined(__POWERPC__))
template<>
inline void random_64bit_wrapper<pcg64>::advance_stream(result_type stream) {
  // set the stream relative to the current stream, i.e. stream = 0 does not change the RNG
  if (stream == 0) return;
  pcg_extras::pcg128_t number;
  std::vector<pcg_extras::pcg128_t> state;
  std::stringstream iss;
  iss << gen;
  using pcg_extras::operator>>;
  while (iss >> number)
    state.push_back(number);
  // state[1] is the current stream
  // PCG will do 2*stream + 1 to make sure stream is odd; need to revert that here by using state[1]/2
  // The first value after changing the stream is often very similar when streams are close-by,
  // see https://github.com/imneme/pcg-cpp/issues/108. So we discard the first draw and also shift `stream`
  // into the upper half, increasing the difference of the used streams w/o changing the logic, where
  // stream=0 means no change. Note that this means that `clone(1).clone(1) != clone(2)`! 
  gen.set_stream(state[1]/2 + (pcg_extras::pcg128_t(stream) << 64) + stream);
  gen();
}

// keep using the two argument ctor for PCG for backwards compatibility
template<>
inline void random_64bit_wrapper<pcg64>::seed(result_type seed, result_type stream) {
  gen.seed(seed, stream);
  cache = false;
}
#endif

inline uint64_t get_seed_from_r() {
  Rcpp::RNGScope rngScope;
  Rcpp::IntegerVector seed(2, dqrng::R_random_int);
  return dqrng::convert_seed<uint64_t>(seed);
}


template<typename RNG = default_64bit_generator>
typename std::enable_if<!std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator () {
  return rng64_t(new random_64bit_wrapper<RNG>(get_seed_from_r()));
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator () {
  return rng64_t(new RNG(get_seed_from_r()));
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<!std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed) {
  return rng64_t(new random_64bit_wrapper<RNG>(seed));
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed) {
  return rng64_t(new RNG(seed));
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<!std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed, uint64_t stream) {
  return rng64_t(new random_64bit_wrapper<RNG>(seed, stream));
}

template<typename RNG = default_64bit_generator>
typename std::enable_if<std::is_base_of<random_64bit_generator, RNG>::value, rng64_t>::type
generator (uint64_t seed, uint64_t stream) {
  return rng64_t(new RNG(seed, stream));
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
