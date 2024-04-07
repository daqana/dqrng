# dqrng (unreleased)

## Breaking changes

* The default RNG has changed from Xoroshiro128+ to Xoroshiro128++. The older generators Xoroshiro128+ and Xoshiro256+ are still available but should only be used for backward compatibility or for generating floating point numbers, i.e. not sampling etc.  ([#57](https://github.com/daqana/dqrng/pull/57) fixing [#56](https://github.com/daqana/dqrng/issues/56))
* The `dqrng::rng64_t` type has been changed to use `Rcpp::XPtr` instead of `std::shared_ptr` and the functions from `dqrng_sample.h` now expect a reference to `dqrng::random_64bit_generator` instead of `dqrng::rng64_t` ([#70](https://github.com/daqana/dqrng/pull/70) fixing [#63](https://github.com/daqana/dqrng/issues/63))

## Other changes

* Decoupled the 'sitmo' package. It is now possible to use, e.g., the distribution functions from the header-only library without having an explicit `LinkingTo: sitmo`.
* Make the internal RNG accessible from the outside (Henrik Sloot fixing [#41](https://github.com/daqana/dqrng/issues/41) in [#58](https://github.com/daqana/dqrng/pull/58))
* Add Xoroshiro128\*\*/++ and Xoshiro256\*\*/++ to `xoshiro.h`
* Allow uniform and normal distributions to be registered as user-supplied RNG within R. This happens automatically if the option `dqrng.register_methods` is set to `TRUE`.
* Add missing inline attributes and limit the included Rcpp headers in `dqrng_types.h` ([#75](https://github.com/daqana/dqrng/pull/75) together with Paul Liétar)
* Add I/O methods for the RNG's internal state (fixing [#66](https://github.com/daqana/dqrng/issues/66) in [#78](https://github.com/daqana/dqrng/pull/78))
* Add `I/O`clone(stream)` method to allow using the global RNG state for parallel computation (fixing [#64](https://github.com/daqana/dqrng/issues/64) in [#79](https://github.com/daqana/dqrng/pull/79))


# dgrng 0.3.2

* Recreate RcppExports.cpp with current development version of Rcpp to fix WARN on CRAN

# dqrng 0.3.1

* new method `dqrrademacher` for drawing Rademacher weights (Kyle Butts in [#50](https://github.com/daqana/dqrng/pull/50) fixing [#49](https://github.com/daqana/dqrng/pull/49))
* Move sampling methods to separate header file, allowing for parallel usage.
* New method `dqrmvnorm` sampling from a multivariate normal distribution.
  This uses the methods implemented in the `mvtnorm` package and uses `dqrnorm`.

# dqrng 0.3.0

## Breaking changes

* The initial state of `dqrng`'s RNG is based on R's RNG, which used to advance R's RNG state.
  The implementation has been changed to preserve R's RNG state, which is less surprising but
  can change the outcome of current scripts.
  ([#44](https://github.com/daqana/dqrng/pull/34) fixing [#43](https://github.com/daqana/dqrng/issues/33))

## Other changes

* For uniform random numbers take short-cut for min == max and throw error for min > max
  ([#34](https://github.com/daqana/dqrng/pull/34) fixing [#33](https://github.com/daqana/dqrng/issues/33))

# dqrng 0.2.1

* Make template specialisations `inline` and include required standard headers (Aaron Lun in [#29](https://github.com/daqana/dqrng/pull/29) fixing [#28](https://github.com/daqana/dqrng/issues/28))
* Add workaraound for new C++ compiler with old libc ([#30](https://github.com/daqana/dqrng/pull/30) fixing [#27](https://github.com/daqana/dqrng/issues/27))
* update maintainer's email address

# dqrng 0.2.0

* Add R side support for selecting multiple streams for parallel usage.
* Implement `long_jump()` for Xo(ro)shiro as alternative to `jump()`
  providing fewer streams with longer period.
* Handle R's RNG scope properly during initialisation.
* New functions `dqsample` and `dqsample.int` using an unbiased sampling
  algorithm.
* Use `R_unif_index()` instead of `unif_rand()` to retrieve random data
  from R's RNG in `generateSeedVectors()`.
* Scalar RNGs in the C++ API: dqrng::runif, dqrng::rnorm and dqrng::rexp

# dqrng 0.1.1

* Use template specializations to avoid compiler warnings during tests (Aaron Lun in [#16](https://github.com/daqana/dqrng/pull/16))
* Do not expect a particular error message on MacOS and skip exception throwing tests on Solaris.

# dqrng 0.1.0

## Breaking changes

* An integer vector instead of a single `int` is used for seeding (Aaron Lun in [#10](https://github.com/daqana/dqrng/pull/10))
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
