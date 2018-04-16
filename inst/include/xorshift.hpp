//  Written in 2014 by Ivo Doko (ivo.doko@gmail.com)
//  based on code written by Sebastiano Vigna (vigna@acm.org)

//  To the extent possible under law, the author has dedicated
//  all copyright and related and neighboring rights to this
//  software to the public domain worldwide. This software is
//  distributed without any warranty.

//  See <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef _XORSHIFT_HPP
#define _XORSHIFT_HPP 1

#if __cplusplus < 201103L
#error This file requires compiler and library support for the ISO C++ 2011 standard.
#endif // __cplusplus

#include <iosfwd>
#include <limits>
#include <type_traits>
#include <cstdint>

  /**
   * @tparam n  The state size (== n * sizeof(UIntType) bytes).
   * @tparam a  First shift parameter.
   * @tparam b  Second shift parameter.
   * @tparam c  Third shift parameter.
   * @tparam m  Scrambling multiplier parameter. If equal to 0,
   *            the "+" variant of the algorithm is used.
   */
template <class UIntType, size_t n,
          int_fast8_t a, int_fast8_t b, int_fast8_t c,
          UIntType m>
class xorshift_engine
{
    static_assert(std::is_unsigned<UIntType>::value,
        "template argument substituting UIntType not an unsigned integral type");
    static_assert(n > 0,
        "template argument substituting n out of bound");
    static_assert(a < std::numeric_limits<UIntType>::digits &&
                  -a < std::numeric_limits<UIntType>::digits,
        "template argument substituting a out of bounds");
    static_assert(b < std::numeric_limits<UIntType>::digits &&
                  -b < std::numeric_limits<UIntType>::digits,
        "template argument substituting b out of bounds");
    static_assert(c < std::numeric_limits<UIntType>::digits &&
                  -c < std::numeric_limits<UIntType>::digits,
        "template argument substituting c out of bounds");

    struct splitmix
    {
        splitmix(const UIntType& k) : state(k) {}

        uint64_t next()
        {
            uint64_t z = (state += 0x9e3779b97f4a7c15ULL);
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
            return z ^ (z >> 31);
        }

    private:
        uint64_t state;
    };

public:

    typedef UIntType result_type;

    static constexpr size_t       word_size    = std::numeric_limits<UIntType>::digits;
    static constexpr size_t       state_size   = n;
    static constexpr int_fast8_t  shift_1      = a >= 0 ? a : -a;
    static constexpr int_fast8_t  shift_2      = b >= 0 ? b : -b;
    static constexpr int_fast8_t  shift_3      = c >= 0 ? c : -c;
    static constexpr result_type  multiplier   = m;
    static constexpr result_type  default_seed = 5489u;

    explicit xorshift_engine (result_type val = default_seed)
    {
        seed(val);
    }

    template <class Sseq, typename = typename
        std::enable_if<!std::is_same<Sseq,
        xorshift_engine>::value>::type>
    explicit xorshift_engine (Sseq& q)
    {
        seed(q);
    }

    void seed (result_type val = default_seed)
    {
        p = 0;
        splitmix init(val);
        for(size_t i = 0; i < state_size; ++i) s[i] = init.next();
    }

    template <class Sseq>
    typename std::enable_if<std::is_class<Sseq>::value>::type
    seed (Sseq& q)
    {
        p = 0;

        if(word_size <= 32)
        {
            q.generate(s, s + state_size);
            return;
        }

        constexpr size_t l = word_size / 32;

        uint32_t _s[l * state_size];

        q.generate(_s, _s + l * state_size);

        for(size_t i = 0; i < state_size; ++i)
        {
            s[i] = 0;

            for(size_t j = 0; j < l; ++j)
                s[i] |= (static_cast<result_type>(_s[i * l + j]) << (j * 32));
        }
    }

    static constexpr result_type min ()
    {
        return n > 1 ? 0 : (m > 0 ? 1 : 0);
    }

    static constexpr result_type max ()
    {
        return std::numeric_limits<result_type>::max();
    }

private:

    result_type s[state_size];
    size_t p;

    template <int_fast8_t k_>
    typename std::enable_if< (k_ >= 0), result_type>::type
    shift(const result_type& val, const int_fast8_t sh)
    {
        return val >> sh;
    }

    template <int_fast8_t k_>
    typename std::enable_if< (k_ < 0), result_type>::type
    shift(const result_type& val, const int_fast8_t sh)
    {
        return val << sh;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ == 1) && (m_ == 0), result_type>::type
    xorshiftgen()
    {
        const result_type s0 = s[0];
        s[0] ^= shift<a>(s[0], shift_1);
        s[0] ^= shift<b>(s[0], shift_2);
        return (s[0] ^= shift<c>(s[0], shift_3)) + s0;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ == 1) && (m_ > 0), result_type>::type
    xorshiftgen()
    {
        s[0] ^= shift<a>(s[0], shift_1);
        s[0] ^= shift<b>(s[0], shift_2);
        return (s[0] ^= shift<c>(s[0], shift_3)) * multiplier;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ == 2) && (m_ == 0), result_type>::type
    xorshiftgen()
    {
        const result_type s0 = s[1], s1 = s[0] ^ shift<a>(s[0], shift_1);
        s[0] = s[1];
        return (s[1] = s1 ^ shift<b>(s1, shift_2) ^
                       s0 ^ shift<c>(s0, shift_3)) + s0;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ == 2) && (m_ > 0), result_type>::type
    xorshiftgen()
    {
        const result_type s1 = s[0] ^ shift<a>(s[0], shift_1);
        s[0] = s[1];
        return (s[1] = s1 ^ shift<b>(s1, shift_2) ^
                       s[1] ^ shift<c>(s[1], shift_3)) * multiplier;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ > 2) && (m_ == 0), result_type>::type
    xorshiftgen()
    {
        const result_type& s0 = s[p];
        p = (p + 1) % state_size;
        const result_type s1 = s[p] ^ shift<a>(s[p], shift_1);
        return (s[p] = s1 ^ shift<b>(s1, shift_2) ^
                       s0 ^ shift<c>(s0, shift_3)) + s0;
    }

    template <size_t n_, result_type m_>
    typename std::enable_if< (n_ > 2) && (m_ > 0), result_type>::type
    xorshiftgen()
    {
        const result_type& s0 = s[p];
        p = (p + 1) % state_size;
        const result_type s1 = s[p] ^ shift<a>(s[p], shift_1);
        return (s[p] = s1 ^ shift<b>(s1, shift_2) ^
                       s0 ^ shift<c>(s0, shift_3)) * multiplier;
    }

public:
    result_type operator() ()
    {
        return xorshiftgen<state_size, multiplier>();
    }

    void discard (unsigned long long z)
    {
        for( ; z > 0; --z) operator()();
    }

    template <class UIntType_, size_t n_,
              int_fast8_t a_, int_fast8_t b_, int_fast8_t c_,
              UIntType_ m_>
    friend bool operator== (const xorshift_engine<UIntType_, n_,
                                                  a_, b_, c_, m_>& lhs,
                            const xorshift_engine<UIntType_, n_,
                                                  a_, b_, c_, m_>& rhs);

    template <class UIntType_, size_t n_,
              int_fast8_t a_, int_fast8_t b_, int_fast8_t c_,
              UIntType_ m_>
    friend bool operator!= (const xorshift_engine<UIntType_, n_,
                                                  a_, b_, c_, m_>& lhs,
                            const xorshift_engine<UIntType_, n_,
                                                  a_, b_, c_, m_>& rhs);

    template <class UIntType_, size_t n_,
              int_fast8_t a_, int_fast8_t b_, int_fast8_t c_,
              UIntType_ m_,
              typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>&
    operator<< (std::basic_ostream<CharT, Traits>& os,
                const xorshift_engine<UIntType_, n_,
                                      a_, b_, c_, m_>& x);

    template <class UIntType_, size_t n_,
              int_fast8_t a_, int_fast8_t b_, int_fast8_t c_,
              UIntType_ m_,
              typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits>&
    operator>> (std::basic_istream<CharT, Traits>& is,
                xorshift_engine<UIntType_, n_,
                                a_, b_, c_, m_>& x);
};

template <class UIntType, size_t n,
          int_fast8_t a, int_fast8_t b, int_fast8_t c,
          UIntType m>
inline bool operator== (const xorshift_engine<UIntType, n,
                                              a, b, c, m>& lhs,
                        const xorshift_engine<UIntType, n,
                                              a, b, c, m>& rhs)
{
    for(size_t i = 0; i < n; ++i)
        if(lhs.s[(lhs.p + i) % n] != rhs.s[(rhs.p + i) % n])
            return false;

    return true;
}

template <class UIntType, size_t n,
          int_fast8_t a, int_fast8_t b, int_fast8_t c,
          UIntType m>
inline bool operator!= (const xorshift_engine<UIntType, n,
                                              a, b, c, m>& lhs,
                        const xorshift_engine<UIntType, n,
                                              a, b, c, m>& rhs)
{
    for(size_t i = 0; i < n; ++i)
        if(lhs.s[(lhs.p + i) % n] != rhs.s[(rhs.p + i) % n])
            return true;

    return false;
}

template <class UIntType, size_t n,
          int_fast8_t a, int_fast8_t b, int_fast8_t c,
          UIntType m,
          typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator<< (std::basic_ostream<CharT, Traits>& os,
            const xorshift_engine<UIntType, n,
                                  a, b, c, m>& x)
{
    typedef std::basic_ostream<CharT, Traits> ostream_type_;
    typedef typename ostream_type_::ios_base  ios_base_;

    const typename ios_base_::fmtflags flags_ = os.flags();
    const CharT fill_ = os.fill();
    const CharT space_ = os.widen(' ');
    os.flags(ios_base_::hex | ios_base_::fixed | ios_base_::left);
    os.fill(space_);

    for(size_t i = 0; i < n; ++i)
        os << x.s[(x.p + i) % n] << space_;

    os.flags(flags_);
    os.fill(fill_);
    return os;
}

template <class UIntType, size_t n,
          int_fast8_t a, int_fast8_t b, int_fast8_t c,
          UIntType m,
          typename CharT, typename Traits>
std::basic_istream<CharT, Traits>&
operator>> (std::basic_istream<CharT, Traits>& is,
            xorshift_engine<UIntType, n,
                            a, b, c, m>& x)
{
    typedef std::basic_istream<CharT, Traits> istream_type_;
    typedef typename istream_type_::ios_base  ios_base_;

    const typename ios_base_::fmtflags flags_ = is.flags();
    is.flags(ios_base_::hex | ios_base_::skipws);

    for(size_t i = 0; i < n; ++i)
        is >> x.s[(x.p + i) % n];

    is.flags(flags_);
    return is;
}

typedef xorshift_engine<uint64_t, 2,
                        -23, 18, 5, 0>           xorshift128plus;

typedef xorshift_engine<uint64_t, 16,
                        -31, 11, 30,
                        1181783497276652981ULL>  xorshift1024star;

#endif // _XORSHIFT_HPP
