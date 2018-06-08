# dqrng 0.0.3.9000

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
