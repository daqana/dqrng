// [[Rcpp::plugins(cpp14)]]
#include <Rcpp.h>
// [[Rcpp::depends(dqrng,BH)]]
#include <dqrng.h>
#include <dqrng_distribution.h>

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dqrexp_extrng(const std::size_t n, const double rate = 1.0) {
  using dist_t = dqrng::exponential_distribution;
  using parm_t = typename dist_t::param_type;

  const auto parm = parm_t{rate};
  auto dist = dist_t{};;
  auto out = Rcpp::NumericVector(Rcpp::no_init(n));

  auto engine = dqrng::random_64bit_accessor{};
  std::generate(out.begin(), out.end(), [&dist, &parm, &engine]() {
    return dist(engine, parm);
  });
  return out;
}
