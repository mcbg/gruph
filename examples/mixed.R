library(Rcpp)
sourceCpp("src/threshold_init.cpp", rebuild = TRUE)
source("R/misc_functions.R")

gen_disc_col <- function(n, k) {
  sample(1:k, n, replace = TRUE)
}

sim_plot <- function(N, M, M2, no_labels) {
  cont <- replicate(M, { rnorm(N) })
  colnames(cont) <- as.character(1:M)
  disc <- replicate(M2 , { gen_disc_col(N, no_labels) })
  colnames(disc) <- (1:M2) + M
  
  edges <- mixed_threshold_init( cont, disc, 0)
  weight_plot(edges, N)
  weight_histogram(edges, N, FALSE)
# return(edges)
}

sim_plot(100, 20, 20, 2) 
sim_plot(100, 20, 20, 3) 
sim_plot(100, 20, 20, 4) 
sim_plot(100, 20, 20, 5) 

sim_plot(100, 2, 20, 5) 
sim_plot(100, 20, 2, 5) 

# mtcars
mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
mtdist <- as.matrix(mtdist)

cont <- setdiff(names(mtcars), c("cyl", "vs", "am", "gear", "carb"))
mtcont <- mtcars[, cont]
mtcont <- as.matrix(mtcont)

edges <- mixed_threshold_init(mtcont, mtdist, 0)

weight_plot(edges,32)
