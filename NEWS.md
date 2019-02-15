# dqrng 0.1.0

## Breaking changes

* An integer vector instead of a single `int` is used for seeding (Aaron Lun in #10)
  * Single integer seeds lead to a different RNG state than before. 
  * `dqrng::dqset_seed()` expects a `Rcpp::IntegerVector` instead of an `int`
* Support for Mersenne-Twister has been removed, Xoroshiro128+ is now the default.

## Other changes

* New method `generateSeedVectors()` for generating a list of random `int`
  vectors from R's RNG. These vectors can be used as seed (Aaron Lun in #10).
* The initial state of the default RNG is now based on R's RNG.

# dqrng 0.0.5

* New RNG: Threefry from package 'sitmo'
* Update PCG Headers (c.f. #8)
* Unit-Tests for the C++ interface
* Define STRICT_R_HEADERS to prepare for future Rcpp (c.f. #6)

# dqrng 0.0.4

* Fix critical bug w.r.t. setting seeds
* Use time in addition to `std::random_device` as source of the default seed, since
  `std::random_device` is deterministic with MinGW (c.f. #2)
* Add jump() method to Xoshiro256+ and Xorohiro128+
* New vignette on parallel usage

# dqrng 0.0.3

* PCG has been patched to compile on Solaris.

# dqrng 0.0.2

* `dqrng_distribution.h` can now be used independently of Rcpp
* Replace `xorshift.hpp` and `xoroshiro.hpp` with `xoshiro.h`.
  This implementation is directly derived from the original C implementations.
  It provides v1.0 of Xoroshiro128+ and Xoshiro256+.

# dqrng 0.0.1

* First public release.
