#ifndef DQRNG_CONVERT_SEED_H_INCLUDED
#define DQRNG_CONVERT_SEED_H_INCLUDED

#include <Rcpp.h>
#include <stdexcept>

/* Create a uint32_t seed using R's PRNGs. */

uint32_t R_random_32 () {
    constexpr double upper_limit=4294967296;
    double val=R::unif_rand() * upper_limit;
    if (val >= upper_limit) { val=0; } // Absolutely avoid overflow.
    return static_cast<uint32_t>(val);
}

/* This is an internal function - not to be called by users. 
 * It converts a seed vector ('seeds') into a single unsigned
 * integer of specified type 'T' with all bits set according
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
    OUT sum=0;
    constexpr OUT upper=-1;
    static_assert(upper > 0, "integer type should be unsigned");

    constexpr TMP max_per_element=-1;
    static_assert(upper >= max_per_element, "unsigned integer type should contain at least 32 bits");

    constexpr OUT left_upper=upper >> TMP_BITS;

    for (size_t i=0; i<N; ++i) {
        OUT current=static_cast<TMP>(seeds[i]);

        if (left_upper < sum) { // avoid overflow upon shift.
            throw std::out_of_range("vector implies an out-of-range seed");
        }
        sum <<= TMP_BITS;

        if (upper - current < sum) { // subtract first, to avoid overflow during check.
            throw std::out_of_range("vector implies an out-of-range seed");
        }
        sum |= current;
    }

    return sum;
}

/* Start of user-visible functions here. 'convert_seed'
 * is overloaded for direct use of input uint32_t's generated
 * by, e.g., std::random_device, to use int's from some other
 * source, or to use int's passed in from R.
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
T convert_seed(SEXP input) {
    Rcpp::IntegerVector seeds(input);
    return convert_seed<T>(seeds.begin(), seeds.size());
}

#endif
