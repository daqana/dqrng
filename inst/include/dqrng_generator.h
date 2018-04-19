#ifndef DQRNG_GENERATOR_H
#define DQRNG_GENERATOR_H 1

#include <cstdint>
#include <memory>
#include <random>
#include <type_traits>

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

typedef std::shared_ptr<random_64bit_generator> rng64_t;

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

template<class RNG = default_64bit_generator>
rng64_t generator (uint64_t seed = std::random_device{}()) {
  return std::make_shared<random_64bit_wrapper<RNG>>(seed);
}
} // namespace dqrng

#endif // DQRNG_GENERATOR_H
