#ifndef DQRNG_CONVERT_SEED_H_INCLUDED
#define DQRNG_CONVERT_SEED_H_INCLUDED

#include <Rcpp.h>
#include <stdexcept>

template<typename T>
T convert_seed(const unsigned char* raws, size_t N) {
    T sum=0;
    constexpr T upper=-1;
    static_assert(upper > 0, "integer type should be unsigned");
    static_assert(upper >= 255, "unsigned integer type should contain at least 8 bits");
    constexpr T left_upper=upper >> 8;

    for (size_t i=0; i<N; ++i) {
        T current=raws[i];
        if (current > 255) {
            throw std::out_of_range("raw vector should only contain values in [0, 255]");
        }

        if (left_upper < sum) { // effectively compare to upper/256, to avoid overflow upon shift.
            throw std::out_of_range("raw vector implies an out-of-range seed");
        }
        sum <<= 8;

        if (upper - current < sum) { // subtract first, to avoid overflow during check.
            throw std::out_of_range("raw vector implies an out-of-range seed");
        }
        sum |= current;
    }

    return sum;
}

template<typename T>
T convert_seed(SEXP input) {
    Rcpp::RawVector raws(input);
    return convert_seed<T>(raws.begin(), raws.size());
}

#endif

