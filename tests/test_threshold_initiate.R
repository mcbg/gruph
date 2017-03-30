Rcpp::sourceCpp("../src/threshold_init.cpp")

context("cont_threshold_init (no penalty)")

test_that("compiles", {
  expect_equal( find('cont_threshold_init'), '.GlobalEnv')
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
  edges <- cont_threshold_init(testdata, lambda = 1.2)
  
  expect_length(attr(edges, "weights"), 2)
})

test_that("dimensions", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 1.2)
  
  expect_equal(ncol(edges), 2)
  expect_equal(nrow(edges), 2)
})