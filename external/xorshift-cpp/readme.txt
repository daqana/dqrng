This project implements xorshift128+ and xorshift1024* algorithms
presented here: http://xorshift.di.unimi.it/

File "reference_xorshift.h" contains the reference implementations
of the two algorithms wrapped in classes.

File "xorshift.hpp" implements these algorithms as typedefs of a
generalised class, implementation of which conforms with the
standard C++11 PRN engine classes in <random> (i.e., it can be
passed as an argument to C++11 distribution objects, used in
standard engine adaptors, etc.).

File "main.cpp" can be used to verify that the provided
implementation produces the same outputs as the reference
implementation. It also demonstrates using xorshift1024* with a
binomial_distribution from <random>.

Currently, the implementation requires the typedef uint64_t to be
available on the target system.


UPDATES:

2016-01-25
    - Library assembled into a single .hpp file.

2016-01-23:
    ! Reseeding the engine now properly resets the state counter.
    ! Relational and stream I/O operators (==, !=, <<, >>) are now
      state-counter-invariant.
    - xorshift64star and xorshift4096star typedefs removed.
    - xorshift128plus parameters updated to conform with the new
      reference implementation.
    - Now using splitmix instead of murmurhash avalanching to
      initialize the state from user-provided seeds (as currently
      recommended by Vigna).
    - Stream output now writes the state values in hexadecimal.
    - Other minor tweaks.
