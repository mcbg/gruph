context("cinit (no penalty)")

test_that("gaussian mutual information", {
  set.seed(12)
  x <- rnorm(50)
  y <- rnorm(50)
  w1 <- - 0.5 * 50 * log(1 - cor(x, y)^2)
  w2 <- attr(cinit(matrix(c(x,y), ncol = 2), 0, TRUE), "weights")
  expect_equal(w1, w2) 
})

test_that("dimensions", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, lambda = 0, TRUE)
  
  expect_equal(ncol(edges), 2)
  expect_equal(nrow(edges), 6)
})


test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, lambda = 0, TRUE)
  
  w <- attr(edges, "weights")
  expect_length(w, 6)
  expect_equal(w[order(-w)], w) # is sorted
})

context("cinit (with penalty)")

test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, lambda = 150 * 0.6, TRUE)
  
  expect_length(attr(edges, "weights"), 2)
})

test_that("dimensions", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, lambda = 150 * 0.6, TRUE)
  
  expect_equal(ncol(edges), 2)
  expect_equal(nrow(edges), 2)
  
  w <- replicate(1000, {
    xx <- matrix(rnorm(100), ncol = 2)
    attr(cinit(xx, lambda = 0, TRUE), "weights")
  })
  w
  quantile(100 * w, c(0.2, 0.5,0.8,0.9))
  qchisq(c(.2,.5,.8, 0.9), 1)
  })
