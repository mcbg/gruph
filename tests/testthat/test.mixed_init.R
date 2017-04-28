context("mixed_threshold_init (no penalty)")

test_that('compiles', {
  expect_equal( find('mixed_threshold_init'), 'package:gruph')
})

test_that('dimensions', {
  mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
  mtdist <- as.matrix(mtdist)
  
  cont <- setdiff(names(mtcars), c("cyl", "vs", "am", "gear", "carb"))
  mtcont <- mtcars[, cont]
  mtcont <- as.matrix(mtcont)
  
  edges <- mixed_threshold_init(mtcont, mtdist, 0)
})
