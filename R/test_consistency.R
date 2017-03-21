library(testthat)

# (1) load functions
Rcpp::sourceCpp("src/threshold_kruskal.cpp")
Rcpp::sourceCpp("src/vanilla_kruskal.cpp")
Rcpp::sourceCpp("src/limited_kruskal.cpp")

test_that("function found", {
  expect_equal( find('threshold_kruskal'), '.GlobalEnv')
  expect_equal( find('vanilla_kruskal'), '.GlobalEnv')
  expect_equal( find('limited_kruskal'), '.GlobalEnv')
})

test_that('implementations are consistent', {
  set.seed(123)
  d <- rnorm(100)
  d <- matrix(d, ncol = 5)
  
  expect_equal(threshold_kruskal(d, 0), vanilla_kruskal(d))
  expect_equal(limited_kruskal(d, 5 - 1), vanilla_kruskal(d))
})
