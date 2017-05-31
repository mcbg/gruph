context("minit (no penalty)")

test_that('compiles', {
  expect_equal( find('minit'), 'package:gruph')
})

test_that('dimensions', {
  mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
  mtdist <- as.matrix(mtdist)
  
  cont <- setdiff(names(mtcars), c("cyl", "vs", "am", "gear", "carb"))
  mtcont <- mtcars[, cont]
  mtcont <- as.matrix(mtcont)
  
  edges <- minit(mtcont, mtdist, 0, FALSE)
})
