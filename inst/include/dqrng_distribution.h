#ifndef DQRNG_DISTRIBUTION_H
#define DQRNG_DISTRIBUTION_H 1

#include <Rcpp.h>
#include <cstdint>
#include <memory>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <dqrng_generator.h>

namespace dqrng {
// Boost's implementation allow for performance increasing spezializations
typedef boost::random::uniform_real_distribution<double> uniform_distribution;
// Boost's implementation uses the fast Ziggurat method
typedef boost::random::normal_distribution<double> normal_distribution;
typedef boost::random::exponential_distribution<double> exponential_distribution;

template<class DIST>
inline Rcpp::NumericVector generate_from_distribution(size_t n, std::shared_ptr<random_64bit_generator> rng, const DIST& dist) {
  Rcpp::NumericVector result(n);
  auto gen = std::bind(dist, std::ref(*rng.get()));
  std::generate(result.begin(), result.end(), gen);
  return result;
}

inline double uniform01(uint64_t x) {
  return (x >> 11) * (1. / (UINT64_C(1) << 53));
}
} // namespace dqrng

namespace boost {
namespace random {
namespace detail {

template<>
inline std::pair<double, int> generate_int_float_pair<double, 8, dqrng::random_64bit_generator>(dqrng::random_64bit_generator& eng)
{
  dqrng::random_64bit_generator::result_type x = eng();
  double r = dqrng::uniform01(x);
  // shift x due to weakness of lowest bits for xorshift/xoroshiro
  int bucket = (x >> 2) & 0xFF;
  return std::make_pair(r, bucket);
}

template<>
inline double generate_uniform_real<dqrng::random_64bit_generator, double>(dqrng::random_64bit_generator& eng, double min, double max)
{
  return dqrng::uniform01(eng()) * (max - min) + min;
}

} // namespace detail
} // namespace random
} // namespace boost

#endif // DQRNG_DISTRIBUTION_H
