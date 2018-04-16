//  Written in 2014 by Ivo Doko (ivo.doko@gmail.com)
//  based on code written by Sebastiano Vigna (vigna@acm.org)

//  To the extent possible under law, the author has dedicated
//  all copyright and related and neighboring rights to this
//  software to the public domain worldwide. This software is
//  distributed without any warranty.

//  See <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <cstdint>

namespace
{
    uint64_t splitmix_state;

    uint64_t splitmix_next()
    {
        uint64_t z = (splitmix_state += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }
}

class ref_xorshift128plus
{
	uint64_t s[2];

public:
	explicit ref_xorshift128plus (const uint64_t& val)
	{
        splitmix_state = val;
        s[0] = splitmix_next();
        s[1] = splitmix_next();
	}

	uint64_t next()
	{
		uint64_t s1 = s[0];
        const uint64_t s0 = s[1];
        s[0] = s0;
        s1 ^= s1 << 23; // a
        s[1] = s1 ^ s0 ^ ( s1 >> 18 ) ^ ( s0 >> 5 ); // b, c
        return s[1] + s0;
	}
};

class ref_xorshift1024star
{
	uint64_t s[16];
    int p = 0;

public:
	explicit ref_xorshift1024star (const uint64_t& val)
	{
        splitmix_state = val;
        for(size_t i = 0; i < 16; ++i)
            s[i] = splitmix_next();
	}

	uint64_t next(void)
	{
        const uint64_t s0 = s[p];
        uint64_t s1 = s[p = (p + 1) & 15];
        s1 ^= s1 << 31; // a
        s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
        return s[p] * 1181783497276652981ULL;
    }
};
