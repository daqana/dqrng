test_that("dqclusterSetRNGStream gives reproducible results w/ setting seed", {
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# do the same again
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are equal
expect_identical(r1, r2)
})

test_that("dqclusterSetRNGStream gives different results w/o setting seed", {
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# do the same again w/o setting the seed
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are not equal
expect_false(identical(r1, r2))
})

test_that("dqclusterSetRNGStream gives different results from different streams", {
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are not equal
expect_false(identical(r1[[1]], r1[[2]]))
})

test_that("dqclusterSetRNGStream w/ substreams gives reproducible results w/ setting seed", {
dqRNGkind("Xoshiro256++")
  # get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# do the same again
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are equal
expect_identical(r1, r2)
})

test_that("dqclusterSetRNGStream w/ substreams gives different results w/o setting seed", {
dqRNGkind("Xoshiro256++")
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# do the same again w/o setting the seed
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are not equal
expect_false(identical(r1, r2))
})

test_that("dqclusterSetRNGStream w/ substreams gives different results from different streams", {
dqRNGkind("Xoshiro256++")
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are not equal
expect_false(identical(r1[[1]], r1[[2]]))
})

test_that("dqclusterSetRNGStream w/ substreams gives different results than w/ streams", {
dqRNGkind("Xoshiro256++")
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, use_substream = TRUE)
r1 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# do the same again w/ streams
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r2 <- parallel::clusterApply(cl, rep(3, 2), dqrng::dqrunif)
parallel::stopCluster(cl)
# make sure results are not equal
expect_false(identical(r1, r2))
})

test_that("dqclusterSetRNGStream gives reproducible results w/ base R", {
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r1 <- parallel::clusterApply(cl, rep(3, 2), stats::runif)
parallel::stopCluster(cl)
# do the same again
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl)
r2 <- parallel::clusterApply(cl, rep(3, 2), stats::runif)
parallel::stopCluster(cl)
# make sure results are equal
expect_identical(r1, r2)
})

test_that("dqclusterSetRNGStream gives different results w/ base R w/o registering", {
# get some random numbers
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, register = FALSE)
r1 <- parallel::clusterApply(cl, rep(3, 2), stats::runif)
parallel::stopCluster(cl)
# do the same again
dqset.seed(123456) 
cl <- parallel::makePSOCKcluster(2)
dqclusterSetRNGStream(cl, register = FALSE)
r2 <- parallel::clusterApply(cl, rep(3, 2), stats::runif)
parallel::stopCluster(cl)
# make sure results are equal
expect_false(identical(r1, r2))
})

test_that("dqrng_advance_stream works for Xoroshiro128+", {
  dqRNGkind("Xoroshiro128+")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works for Xoroshiro128++", {
  dqRNGkind("Xoroshiro128++")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works (but differently) for PCG64", {
  dqRNGkind("PCG64")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  # PCG64 streams are not "additive"
  expect_false(identical(r1, r2))
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r2 <- dqrunif(10)
  # but they are still reproducible
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works for Xoshiro256+", {
  dqRNGkind("Xoshiro256+")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works for Xoshiro256++", {
  dqRNGkind("Xoshiro256++")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works for MWC192X", {
  dqRNGkind("MWC192X")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream works for Threefry", {
  dqRNGkind("Threefry")
  dqset.seed(123456) 
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  dqrng_advance_stream(1)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})


test_that("dqrng_advance_stream with substreams produces error for Xoroshiro128+",{
  dqRNGkind("Xoroshiro128+")
  expect_error(dqrng_advance_stream(1, use_substream = TRUE), "Substream handling not supported for this RNG!")
})

test_that("dqrng_advance_stream with substreams produces error for Xoroshiro128++",{
  dqRNGkind("Xoroshiro128++")
  expect_error(dqrng_advance_stream(1, use_substream = TRUE), "Substream handling not supported for this RNG!")
})

test_that("dqrng_advance_stream with substreams produces error for PCG64",{
  dqRNGkind("PCG64")
  expect_error(dqrng_advance_stream(1, use_substream = TRUE), "Substream handling not supported for this RNG!")
})

test_that("dqrng_advance_stream with substreams works for Xoshiro256+", {
  dqRNGkind("Xoshiro256+")
  dqset.seed(123456) 
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3, use_substream = TRUE)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream with substreams works for Xoshiro256++", {
  dqRNGkind("Xoshiro256++")
  dqset.seed(123456) 
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3, use_substream = TRUE)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream with substreams works for MWC192X", {
  dqRNGkind("MWC192X")
  dqset.seed(123456) 
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3, use_substream = TRUE)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})

test_that("dqrng_advance_stream with substreams works for Threefry", {
  dqRNGkind("Threefry")
  dqset.seed(123456) 
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  dqrng_advance_stream(1, use_substream = TRUE)
  r1 <- dqrunif(10)
  dqset.seed(123456) 
  dqrng_advance_stream(3, use_substream = TRUE)
  r2 <- dqrunif(10)
  expect_identical(r1, r2)
})
