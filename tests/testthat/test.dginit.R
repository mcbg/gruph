context("dginit (no penalty)")

test_that('compiles', {
  expect_equal(find('dinit'), 'package:gruph')
})

test_that('is the same as cinit without zeroes', {
  set.seed(12)
  xx <- rnorm(100)
  dim(xx) <- c(10, 10)
  
  expect_equal(dginit(xx, 0, TRUE), cinit(xx, 0, TRUE)) 
})

test_that('no Inf/-Inf with zeroes.', {
  set.seed(12)
  xx <- rnorm(100)
  zeros_index <- sample(c(TRUE,FALSE), 100, replace = TRUE)
  xx[zeros_index] <- 0
  dim(xx) <- c(10, 10)
  
  w <- weights(dginit(xx, 0, TRUE))
  
  expect_false(any(c(Inf,-Inf) %in% w))
}) 
  