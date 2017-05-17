context("max_forest")

# TODO: RELIES ON CINIT CHANGE THIS!!!!
test_that("dims", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, 0, TRUE) 
   
  f_edges <- max_forest(edges)
  
  expect_equal(ncol(f_edges), 2)
  expect_equal(nrow(f_edges), 3)
})


test_that("weights", {
  testdata <- iris[, -5]  
  testdata <- as.matrix(testdata)
  edges <- cinit(testdata, lambda = 0, TRUE)
  f_edges <- max_forest(edges)
  
  expect_length(attr(f_edges, "weights"), 3)
})
