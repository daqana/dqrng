#ifndef DQRNG_GENERATOR_H
#define DQRNG_GENERATOR_H 1

#include <Rcpp.h>
#include <cstdint>
#include <memory>
#include <random>
#include <type_traits>
#include <xorshift.hpp>
#include <xoroshiro.hpp>
#include <pcg_random.hpp>


namespace dqrng {
// conservative default
typedef std::mt19937_64 default_64bit_generator;

struct random_64bit_generator {
  typedef uint64_t result_type;

  virtual ~random_64bit_generator() {};
  virtual result_type operator() () = 0;
  virtual void seed(result_type seed) = 0;
  virtual result_type min() {return 0.0;};
  virtual result_type max() {return UINT64_MAX;};
};

template<class RNG>
struct random_64bit_wrapper : random_64bit_generator {
  static_assert(std::is_same<random_64bit_generator::result_type, typename RNG::result_type>::value,
                "Provided RNG has wrong result_type");
  static_assert(RNG::max() == UINT64_MAX, "Provided RNG has wrong maximum.");
  static_assert(RNG::min() == 0, "Provided RNG has wrong minimum.");
  RNG gen;

  virtual result_type operator() () {return gen();}
  virtual void seed(result_type seed) {gen.seed(seed);}
};

inline std::shared_ptr<random_64bit_generator> generator(const std::string kind, uint64_t seed) {
  std::shared_ptr<random_64bit_generator> rng;
  if (kind == "DEFAULT") {
    rng = std::make_shared<random_64bit_wrapper<default_64bit_generator>>();
  } else if (kind == "MERSENNE-TWISTER") {
    rng = std::make_shared<random_64bit_wrapper<std::mt19937_64>>();
  } else if (kind == "XORSHIFT128+") {
    rng = std::make_shared<random_64bit_wrapper<xorshift128plus>>();
  } else if (kind == "XORSHIFT1024*") {
    rng = std::make_shared<random_64bit_wrapper<xorshift1024star>>();
  } else if (kind == "XOROSHIRO128+") {
    rng = std::make_shared<random_64bit_wrapper<xoroshiro128plus_engine>>();
  } else if (kind == "PCG64") {
    rng = std::make_shared<random_64bit_wrapper<pcg64>>();
  } else {
    Rcpp::stop("Unknown random generator kind.");
  }
  rng->seed(seed);
  return rng;
}

inline std::shared_ptr<random_64bit_generator> generator(const std::string kind = "DEFAULT") {
  std::random_device r;
  uint64_t seed(r());
  return generator(kind, seed);
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
