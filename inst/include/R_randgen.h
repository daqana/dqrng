#ifndef DQRNG_R_RANDGEN_H_INCLUDED
#define DQRNG_R_RANDGEN_H_INCLUDED

#include <Rcpp.h>

namespace dqrng {

/* Create a uint32_t or int seed using R's PRNGs.
 * The uint32_t seed is more convnient from use within C++,
 * the int seed is more convenient if it needs to be passed back to R.
 */

inline uint32_t R_random_u32 () {
    Rcpp::RNGScope rngScope;
    constexpr double upper_limit=4294967296;
    double val = R_unif_index(upper_limit);
    if (val >= upper_limit) { val=0; } // Absolutely avoid overflow.
    return static_cast<uint32_t>(val);
}

inline int R_random_int () {
    const uint32_t sampled=R_random_u32();
    constexpr uint32_t max_int=2147483647; // See .Machine$integer.max.
    if (sampled <= max_int) {
        return static_cast<int>(sampled);
    }

    // Effectively reverse of the int->uint32_t cast.
    constexpr uint32_t max_uint=-1;
    return -static_cast<int>(max_uint - sampled) - 1;
}

}

#endif
