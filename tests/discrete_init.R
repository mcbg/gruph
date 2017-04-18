Rcpp::sourceCpp("../src/threshold_init.cpp")

context("discrete_threshold_init (no penalty)")

test_that('compiles', {
  expect_equal( find('discrete_threshold_init'), '.GlobalEnv')
})

test_that('correct dimensions', {
  # 1) discrete variables of `mtcars`
  mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
  mtdist <- as.matrix(mtdist)
  
  edges <- discrete_threshold_init(mtdist, 0)
  
  
  w <- attr(edges, "weights")
  df <- attr(edges, "df")
 
  expect_length(w, 10)
  expect_length(df, 10)
  
  expect_equal(w, w[order(-w)]) # check weights are ordered
})

  