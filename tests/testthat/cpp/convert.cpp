// [[Rcpp::plugins(cpp11)]]

#include <Rcpp.h>
// [[Rcpp::depends(dqrng)]]
#include "convert_seed.h"

#include <string>
#include <sstream>

template<typename U>
std::string convert_base(Rcpp::IntegerVector seed) {
    U out=dqrng::convert_seed<U>(seed);
    std::stringstream ss;
    ss << out;
    return ss.str();
}

// [[Rcpp::export]]
std::string convert_16(Rcpp::IntegerVector seed) {
    return convert_base<uint16_t>(seed);
}

// [[Rcpp::export]]
std::string convert_32(Rcpp::IntegerVector seed) {
    return convert_base<uint32_t>(seed);
}

// [[Rcpp::export]]
std::string convert_64(Rcpp::IntegerVector seed) {
    return convert_base<uint64_t>(seed);
}

// [[Rcpp::export]]
bool is_signed_consistent(Rcpp::IntegerVector seed) {
    uint64_t val=dqrng::convert_seed<uint64_t>(seed);

    std::vector<uint32_t> casted(seed.begin(), seed.end());
    uint64_t comp=dqrng::convert_seed<uint64_t>(casted.data(), casted.size());

    return (val==comp);
}
