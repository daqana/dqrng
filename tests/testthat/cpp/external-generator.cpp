// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
// [[Rcpp::depends(dqrng,sitmo,BH)]]
#include <dqrng.h>
#include <mystdint.h>
#include <Rcpp.h>
#include <dqrng_generator.h>
#include <dqrng_distribution.h>
#include <xoshiro.h>
#include <pcg_random.hpp>
#include <threefry.h>
#include <convert_seed.h>
#include <R_randgen.h>
#include <minimal_int_set.h>

template<typename _Distribution>
struct distribution_caller {
  using dist_t = _Distribution;
  using parm_t = typename dist_t::param_type;
  template<typename _Engine, typename... Args>
  static Rcpp::NumericVector call(_Engine&& engine, const std::size_t n, Args&&... args) {
    const auto parm = parm_t{std::forward<decltype(args)>(args)...};
    auto dist = dist_t{};
    auto out = Rcpp::NumericVector(Rcpp::no_init(n));
    std::generate(out.begin(), out.end(), [&dist, &parm, &engine]() {
      return dist(engine, parm);
    });
    return out;
  }
};

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector test_dqrunif(const std::size_t n, const double min = 0.0, const double max = 1.0) {
  using caller_t = distribution_caller<dqrng::uniform_distribution>;

  auto rng = dqrng::get_rng();
  return caller_t::call(*rng, n, min, max);
}

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector test_dqrexp(const std::size_t n, const double rate = 1.0) {
  using caller_t = distribution_caller<dqrng::exponential_distribution>;

  auto rng = dqrng::get_rng();
  return caller_t::call(*rng, n, rate);
}

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector test_dqrnorm(const std::size_t n, const double mean = 0.0, const double sd = 1.0) {
  using caller_t = distribution_caller<dqrng::normal_distribution>;

  auto rng = dqrng::get_rng();
  return caller_t::call(*rng, n, mean, sd);
}
