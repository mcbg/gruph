context("fl_dginit")

test_that("normal usage works", {
  iris_cont <- data.matrix(iris[, 1:4])
  iris_dis <- data.matrix(iris[, 5, drop = FALSE])
  
  g <- fl_dginit(iris_cont, iris_dis, 200, 0, verbose = FALSE)
  expect_equal(nrow(g), 10)
  
  g2 <- fl_dginit(iris_cont, iris_dis, 3, 0, verbose = FALSE)
  expect_equal(nrow(g2), 3)
})

test_that("correlation undefinied", {
  iris_cont <- data.matrix(iris[, 1:4])
  iris_dis <- data.matrix(iris[, 5, drop = FALSE])
  iris_cont[,2] <- 0

    
  g <- fl_dginit(iris_cont, iris_dis, 200, 0, verbose = FALSE)
  expect_true(all(!is.na(g$weights)))
  
  iris_cont[1,2] <- 1
  g2 <- fl_dginit(iris_cont, iris_dis, 200, 0, verbose = FALSE)
  expect_true(all(!is.na(g2$weights)))
})

test_that("handles A = B", {
  iris_cont <- data.matrix(iris[, 1:4])
  iris_dis <- data.matrix(iris[, 5, drop = FALSE])
  iris_cont[1:5,2] <- 0
  iris_cont[1:5,3] <- 0

  g <- fl_dginit(iris_cont, iris_dis, 200, 0, verbose = FALSE)
  expect_true(all(!is.na(g$weights)))
})