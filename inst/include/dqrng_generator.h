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
private:
  RNG gen;

public:
  random_64bit_wrapper() : gen() {};
  random_64bit_wrapper(result_type seed) : gen(seed) {};
  virtual result_type operator() () {return gen();}
  virtual void seed(result_type seed) {gen.seed(seed);}
};

inline std::shared_ptr<random_64bit_generator> generator(std::string kind, uint64_t seed) {
  for (auto & c: kind)
    c = std::toupper(c);
  if (kind == "DEFAULT") {
    return std::make_shared<random_64bit_wrapper<default_64bit_generator>>(seed);
  } else if (kind == "MERSENNE-TWISTER") {
    return std::make_shared<random_64bit_wrapper<std::mt19937_64>>(seed);
  } else if (kind == "XORSHIFT128+") {
    return std::make_shared<random_64bit_wrapper<xorshift128plus>>(seed);
  } else if (kind == "XORSHIFT1024*") {
    return std::make_shared<random_64bit_wrapper<xorshift1024star>>(seed);
  } else if (kind == "XOROSHIRO128+") {
    return std::make_shared<random_64bit_wrapper<xoroshiro128plus_engine>>(seed);
  } else if (kind == "PCG64") {
    return std::make_shared<random_64bit_wrapper<pcg64>>(seed);
  } else {
    Rcpp::stop("Unknown random generator kind.");
  }
}

inline std::shared_ptr<random_64bit_generator> generator(const std::string kind = "DEFAULT") {
  std::random_device r;
  uint64_t seed(r());
  return generator(kind, seed);
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
