//  Written in 2014 by Ivo Doko (ivo.doko@gmail.com)

//  To the extent possible under law, the author has dedicated
//  all copyright and related and neighboring rights to this
//  software to the public domain worldwide. This software is
//  distributed without any warranty.

//  See <http://creativecommons.org/publicdomain/zero/1.0/>.

#include <iostream>
#include "reference_xorshift.h"
#include "xorshift.hpp"
#include <random>
#include <array>
#include <functional>

using namespace std;

uint64_t seedval{0}, trynum{0}, differences{0};

template<class A, class B>
void compare()
{
    A en1{seedval};
    B en2{seedval};
    for(size_t i = 0; i < trynum; ++i)
    {
        if(en1() != en2.next()) ++differences;
    }
}

int main()
{
    while(seedval == 0)
    {
        cout << "Enter a seed value (positive integer): ";
        cin >> seedval;
    }

    while(trynum == 0)
    {
        cout << "Enter the number of values to be tested: ";
        cin >> trynum;
    }

    compare<xorshift128plus, ref_xorshift128plus>();
    compare<xorshift1024star, ref_xorshift1024star>();

    cout << endl << "Number of different outputs: " << differences << endl << endl;

    cout << "Binomial distribution using xorshift1024* with the provided seed:" << endl << endl;

    xorshift1024star engine{seedval};
    binomial_distribution<size_t> distribution(9, 0.5);
    auto rnd = bind(distribution, engine);

    array<size_t, 10> res;
    res.fill(0);

    for(size_t i = 0; i < 200; ++i) ++res[rnd()];

    for(size_t i = 0; i < 10; ++i)
        cout << i << ": " << std::string(res[i], '*') << endl;

    return 0;
}
