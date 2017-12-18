context("dinit (no penalty)")

test_that('correct dimensions', {
  # 1) discrete variables of `mtcars`
  mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
  mtdist <- as.matrix(mtdist)
  
  edges <- dinit(mtdist, 0, TRUE)
  
  
  w <- attr(edges, "weights")
  df <- attr(edges, "df")
 
  expect_length(w, 10)
  expect_length(df, 10)
  
  expect_equal(w, w[order(-w)]) # check weights are ordered
})

  
