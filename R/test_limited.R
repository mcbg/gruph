library(testthat)
Rcpp::sourceCpp("src/limited_kruskal.cpp")

test_that('basic tests for limited', {
  expect_equal( find('limited_kruskal'), '.GlobalEnv')
  
  set.seed(123)
  d <- rnorm(100)
  d <- matrix(d, ncol = 5)
  edges <- limited_kruskal(d, 3)
  
  expect_equal( nrow(edges), 3)
})
