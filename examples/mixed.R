Rcpp::sourceCpp("src/threshold_init.cpp")

# random variables

gen_disc_col <- function(n, k) {
  sample(1:k, n, replace = TRUE)
}

sim_plot <- function(N, M, M2, no_labels) {
  set.seed(12)
  cont <- replicate(M, { rnorm(N) })
  colnames(cont) <- as.character(1:M)
  disc <- replicate(M2 , { gen_disc_col(N, no_labels) })
  colnames(disc) <- (1:M2) + M
  
  edges <- mixed_threshold_init( cont, disc, 0)
  #w <- attr(edges, "weights")
  weight_plot(edges, N)
  table(attr(edges, "df"))
  return(edges)
}


sim_plot(100, 20, 10, 2)
sim_plot(100, 20, 10, 3) 
sim_plot(100, 20, 10, 4) 



# mtcars
mtdist <- mtcars[, c("cyl", "vs", "am", "gear", "carb")]
mtdist <- as.matrix(mtdist)

cont <- setdiff(names(mtcars), c("cyl", "vs", "am", "gear", "carb"))
mtcont <- mtcars[, cont]
mtcont <- as.matrix(mtcont)

edges <- mixed_threshold_init(mtcont, mtdist, 0)

weight_plot(edges,32)
