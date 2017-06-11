sim_gene_data <- function(n = 1000, k = 1000) {
  calc_column <- function() {
    x <- rnorm(k)
    s <- sample(c(TRUE, FALSE), k, replace = TRUE)
    x[s] <- 0
    x
  }
  replicate(n, calc_column())
} 


sim_discrete_data <- function(n = 1000, k = 1000) {
  calc_column <- function() {
    s <- sample(c(1, 2), k, replace = TRUE)
    s
  }
  replicate(n, calc_column())
} 