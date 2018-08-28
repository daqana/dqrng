#include <testthat.h>
#include <xoshiro.h>

context("xoshiro jump()") {
  test_that("jump() for xoroshiro128+ works") {
    dqrng::xoroshiro128plus rng1;
    dqrng::xoroshiro128plus rng2{rng1};
    expect_true(rng1() == rng2());
    rng2.jump(1);
    expect_true(rng1() != rng2());
  }

  test_that("jump() for xoshiro256+ works") {
    dqrng::xoshiro256plus rng1;
    dqrng::xoshiro256plus rng2{rng1};
    expect_true(rng1() == rng2());
    rng2.jump(1);
    expect_true(rng1() != rng2());
  }
}
