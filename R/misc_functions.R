name_edges <- function(edges, xx) {
  out <- colnames(xx)[edges]
  dim(out) <- dim(edges)
  out
}

library(ggplot2)
weight_plot <- function(edges, N) {
  # N: number of observations
  
  get_p_value <- function(w, degrees) pchisq(2 * N * w, degrees, lower.tail = TRUE)
  w <- attr(edges, "weights")
  dfs <- attr(edges, "df")
  pvals <- unlist(Map(get_p_value, w, dfs))
  
  xx <- data.frame(index = (1:length(pvals))/length(pvals), p = pvals)
  ggplot(aes(x = index, y = p), data = xx) + geom_point() +
    geom_abline(slope = -1, intercept = 1, col = "red", size = 1.2) +
    xlim(0, 1) + ylim(0, 1)
}

