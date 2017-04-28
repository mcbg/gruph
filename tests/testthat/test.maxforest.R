context("max_forest")

test_that("dims", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0)
  f_edges <- max_forest(edges, attr(edges, "weights"))
  
  expect_equal(ncol(f_edges), 2)
  expect_equal(nrow(f_edges), 3)
})


test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cont_threshold_init(testdata, lambda = 0)
  f_edges <- max_forest(edges, attr(edges, "weights"))
  
  expect_length(attr(f_edges, "weights"), 3)
})
