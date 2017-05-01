library(igraph)
library(gruph)

gen_disc_col <- function(n, k) {
  sample(1:k, n, replace = TRUE)
}

mktest <- function(N, M, M2, no_labels) {
  cat(Sys.time(),  "\n")
  cont <- replicate(M, { rnorm(N) })
  colnames(cont) <- as.character(1:M)
  disc <- replicate(M2 , { gen_disc_col(N, no_labels) })
  colnames(disc) <- (1:M2) + M
  list(cont = cont, disc = disc)
}

sim <- function(N, M, M2, no_labels, lambda = 0) {
  cat(Sys.time(),  "\n")
  cont <- replicate(M, { rnorm(N) })
  colnames(cont) <- as.character(1:M)
  disc <- replicate(M2 , { gen_disc_col(N, no_labels) })
  colnames(disc) <- (1:M2) + M
  
  edges <- mixed_threshold_init( cont, disc, lambda)
  cat(Sys.time(),  "\n")
  edges
}

t1 <- mktest(800, 800, 25, 2)
t2 <- mktest(800, 1600, 25, 2)

hi2 <- microbenchmark::microbenchmark(
 mixed_threshold_init(t1$cont, t1$disc, 0),
 mixed_threshold_init(t2$cont, t2$disc, 0),
 mixed_threshold_init(t1$cont, t1$disc, 0.00393),
 mixed_threshold_init(t2$cont, t2$disc, 0.00393),
 times = 3, unit = "s"
)

hi2

hi <- microbenchmark::microbenchmark(
  sim(800, 800, 25, 2, 0.00393),
  sim(800, 1600, 25, 2, 0.00393), 
  sim(800, 3200, 25, 2, 0.00393), 
  times = 3, unit = "s"
)

hi
x <- c(100, 200, 400, 800, 1600, 3200)
y <- c(0.3, 0.8, 2.2, 6.7, 22, 82)
plot(x, y)
ml <- lm(y ~ x + I(x^2))

est <- predict(ml, data.frame(x = 50000))
est/ (60*60)

e <- sim(800, 600, 25, 2, 0)

e <- sim(800, 100, 25, 2, 0)
e <- sim(800, 100, 25, 2, 0.00393)
weight_plot(e, 800)
e <- max_forest(e)
g <- graph.edgelist(e + 1)
pvals <- calc_pval(e, 800)

V(g)$size <- 4
V(g)$label <- ""
V(g)[101:125]$size <- 6
V(g)[101:125]$color <- "blue"
V(g)$label.cex <- 1.3
E(g)$color <- "black"
E(g)$color <- "darkgrey"
E(g)[pvals > 0.99]$color <- "black"
E(g)[pvals > 0.999]$color <- "red"
E(g)$arrow.mode <- "-"
E(g)$arrow.size <- 2

gn <- make_ego_graph(g, 3, 124)[[1]]

#l <- layout.fruchterman.reingold(g,niter=500) 
l <- layout_nicely(g)
plot(g, layout = l, edge.width = 2 ,margin = -0.2)
plot(gn)
