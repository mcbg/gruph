# Weight plot
Rcpp::sourceCpp("src/threshold_init.cpp")

# continuous
set.seed(123)

N <- 100
M <- 100

df <- rnorm(n = N * M)
dim(df) <- c(N, M)
edges <- cont_threshold_init(df, 0)
w <- attr(edges, "weights")
ps <- pchisq(2 *  N * w, 1, lower.tail = TRUE)
ggplot2::qplot(ps) # SICKENING!

b <- (1:length(ps)) / length(ps)
ggplot2::qplot(b, ps) +
  geom_abline(slope = -1, intercept = 1, col = "red", size = 1.2)# SICKENING! 

#discrete 
df <- rbinom(N * M, 1, 0.5)
dim(df) <- c(N, M)

edges <- multivariate_threshold_init(df, 0)
w <- attr(edges, "weights")
ps <- pchisq(2 * N * w, 1, lower.tail = TRUE)
b <- (1:length(ps)) / length(ps)
ggplot2::qplot(b, ps) +
  geom_abline(slope = -1, intercept = 1, col = "red", size = 1.2)# SICKENING! 

