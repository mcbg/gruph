library(testthat)
Rcpp::sourceCpp("src/threshold_kruskal.cpp")

test_that("function found", {
  expect_equal( find('threshold_kruskal'), '.GlobalEnv')
})

test_that('', {
  data("carcass")
  set.seed(123)
  
  d <- rnorm(100)
  d <- matrix(d, ncol = 5)
  cor(d)
  threshold_kruskal(d, 0.05) + 1
})