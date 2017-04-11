# Weight plot
library(ggplot2)

Rcpp::sourceCpp("src/threshold_init.cpp")

plot_weights <- function(w, probs = seq(0.1, .9, 0.1)) {
  df <- data.frame(
    obs = quantile(w, probs = probs),
    teoretical = qchisq(probs, 1)
  )
  
  ggplot(aes(x = teoretical, y = obs), data = df) +
    geom_point() +
    geom_abline(slope = 1, size = 1.5, col = "red") 
}


w <- attr(edges, "weights")
w <- attr(cont_threshold_init(matrix(rnorm(1500), ncol = 150), 0), "weights")
qplot(pchisq(w * 4, 1, log.p = FALSE), geom= "histogram")
qplot(pchisq(w * 4, 1, lower.tail = FALSE, log.p = FALSE), geom= "histogram")

- 0.5 * log(1 - cor(x, y)^2)
attr(cont_threshold_init(matrix(c(x,y), ncol = 2), 0), "weights")

a <- replicate(1000, {
  x <- rnorm(50)
  y <- x + rnorm(50)
  c(- 0.5 * log(1 - cor(x, y)^2),
    cor(x,y), 
  t.test(x,y)$statistic  )
})

qplot(a[2, ], a[3, ]) + xlim(0,1) + ylim(0,1)

f <- function(x) pchisq(x, 1, lower.tail = FALSE)
qplot(f(a))

attr(cont_threshold_init(matrix(rnorm(50), ncol = 2), 0), "weights")
qplot(pchisq(x  , 1, lower.tail = FALSE, log.p = FALSE))
qplot(pchisq(x  , 1, lower.tail = TRUE, log.p = FALSE))

x <- replicate(1000, { t.test(rnorm(100), rnorm(100))$p.value })
qplot(x)
w <- attr(edges, "weights")
plot_weights(w)


