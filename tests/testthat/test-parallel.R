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
# make sure results are equal
expect_false(identical(r1, r2))
})
