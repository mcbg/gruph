#include <Rcpp.h>
#include <vector>
#include "stats.h"
using namespace Rcpp;

typedef std::vector<double> vec;

// [[Rcpp::export]]
NumericVector cpp_sd(NumericVector px) {
  vec x = as<vec>(px);
  stats_functions<vec> stats;
  return wrap(stats.sd(x));
}

// [[Rcpp::export]]
NumericVector cpp_cor(NumericVector px, NumericVector py) {
  vec x = as<vec>(px);
  vec y = as<vec>(py);
  stats_functions<vec> stats;
  return wrap(stats.cor(x, y));
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically 
// run after the compilation.
//

/*** R
library(testthat)

set.seed(123)
x <- rnorm(100)
y <- rnorm(100)

test_that("sd works", {
  expect_equal(sd(x), cpp_sd(x))
})

test_that("cor works", {
  expect_equal(cor(x, y), cpp_cor(x, y))
})
*/
