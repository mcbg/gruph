Rcpp::sourceCpp("../src/threshold_init.cpp")

context("cont_threshold_init (no penalty)")

test_that("compiles", {
  expect_equal( find('cont_threshold_init'), '.GlobalEnv')
})

test_that("gaussian mutual information", {
  set.seed(12)
  x <- rnorm(50)
  y <- rnorm(50)
  w1 <- - 0.5 * log(1 - cor(x, y)^2)
  w2 <- attr(cont_threshold_init(matrix(c(x,y), ncol = 2), 0), "weights")
  expect_equal(w1, w2) 
})

test_that("dimensions", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0)
  
  expect_equal(ncol(edges), 2)
  expect_equal(nrow(edges), 6)
})

test_that("elements", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0)
  
  expect_equal(min(edges), 1)
})

test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0)
  
  w <- attr(edges, "weights")
  expect_length(w, 6)
  expect_equal(w[order(-w)], w) # is sorted
})

context("cont_threshold_init (with penalty)")

test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0.6)
  
  expect_length(attr(edges, "weights"), 2)
})

test_that("dimensions", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0.6)
  
  expect_equal(ncol(edges), 2)
  expect_equal(nrow(edges), 2)
})