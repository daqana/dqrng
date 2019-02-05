#ifndef DQRNG_CONVERT_SEED_H_INCLUDED
#define DQRNG_CONVERT_SEED_H_INCLUDED

#include <Rcpp.h>
#include <stdexcept>

/* Create a uint32_t or int seed using R's PRNGs. 
 * The uint32_t seed is more convnient from use within C++,
 * the int seed is more convenient if it needs to be passed back to R.
 */

inline uint32_t R_random_u32 () {
    constexpr double upper_limit=4294967296;
    double val=R::unif_rand() * upper_limit;
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

/* This is an internal function - not to be called by users. 
 * It converts a seed vector ('seeds') into a single unsigned
 * integer of specified type 'OUT' with all bits set according
 * to the combined bit pattern of the individual seed elements.
 * This is achieved by bit shifting, with the first element
 * of 'seeds' contributing the most significant bits in the output
 * and the last element contributing the least significant bits.
 *
 * Each element of 'seeds' is cast from 'IN' to 'TMP' prior to
 * bit operations. This is intended to eliminate signed-ness,
 * so 'TMP' should be the unsigned equivalent of 'int' with the
 * same number of bits (as specified by 'TMP_BITS').
 */

template<typename OUT, typename IN, typename TMP, unsigned int TMP_BITS>
OUT convert_seed_internal(const IN* seeds, size_t N) {
    constexpr OUT upper=-1;
    static_assert(upper > 0, "output integer type should be unsigned");

    constexpr TMP max_tmp=-1;
    static_assert(max_tmp > 0, "temporary integer type should be unsigned");
    static_assert(upper >= max_tmp, "output integer type should not be smaller than the temporary type");

    // Check to avoid UB from right-shifting by the length of OUT in bits.
    constexpr bool not_same=upper > max_tmp;
    constexpr OUT left_upper=(not_same ? upper >> TMP_BITS : 0);

    OUT sum=0;
    for (size_t i=0; i<N; ++i) {
        OUT current=static_cast<TMP>(seeds[i]);

        if (left_upper < sum) { // avoid overflow upon shift.
            throw std::out_of_range("vector implies an out-of-range seed");
        }
        if (not_same) { // avoid UB from left-shifting by the length of OUT in bits.
            sum <<= TMP_BITS;
        }

        if (upper - current < sum) { // subtract first, to avoid overflow during check.
            throw std::out_of_range("vector implies an out-of-range seed");
        }
        sum |= current;
    }

    return sum;
}

/* Re-start of user-visible functions here. 'convert_seed'
 * is overloaded for direct use of input uint32_t's generated
 * by, e.g., std::random_device; to use int's from some other
 * source; or to use int's passed in from R.
 */

template<typename T>
T convert_seed(const uint32_t* seeds, size_t N) {
    return convert_seed_internal<T, uint32_t, uint32_t, 32>(seeds, N);
}

template<typename T>
T convert_seed(const int* seeds, size_t N) {
    return convert_seed_internal<T, int, uint32_t, 32>(seeds, N); // all ints are 32-bit on platforms that compile R.
}

template<typename T>
T convert_seed(Rcpp::IntegerVector seeds) {
    return convert_seed<T>(seeds.begin(), seeds.size());
}

#endif
