context("dminit (no penalty, independent simulation)")

N <- 100

gen_column <- function() {
  x <- rnorm(N)
  s <- sample(c(TRUE, FALSE), N, replace = TRUE)
  x <- ifelse(s, x, 0)
  x
}

x <- matrix(replicate(100, { gen_column() }), 100)
y <- matrix(replicate(100, { sample(1:2, 100, replace = TRUE) }), 100)

colnames(x) <- seq(1,100)
colnames(y) <- seq(101,200)

test_that("dimensions", {
  edges <- dminit(x, y, 0, FALSE)
  edges2 <- minit(x, y, 0, FALSE)
  table(dfs(edges))
  weight_plot(edges, 100)
  weight_plot(edges2, 100)
  expect_equal(nrow(edges), nrow(edges2)) # 200 * 199 / 2 expected edges
})
