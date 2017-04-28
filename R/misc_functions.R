library(ggplot2)

name_edges <- function(edges, xx) {
  out <- colnames(xx)[edges]
  dim(out) <- dim(edges)
  out
}

dfs <- function(edges) {
 attr(edges, "df") 
}

weights <- function(edges) {
 attr(edges, "weights") 
}

weight_plot <- function(edges, N) {
  # N: number of observations
  
  get_p_value <- function(w, degrees) pchisq(2 * N * w, degrees, lower.tail = TRUE)
  w <- attr(edges, "weights")
  dfs <- attr(edges, "df")
  pvals <- unlist(Map(get_p_value, w, dfs))
  
  xx <- data.frame(index = (1:length(pvals))/length(pvals),
                   p = pvals,
                   df = factor(dfs))
  
  ggplot(aes(x = index, y = p, col = df), data = xx) +
    geom_point() +
    geom_abline(slope = -1, intercept = 1, col = "red", size = 1.2) 
    #facet_wrap(~df) #+ xlim(0, 1) + ylim(0, 1) 
  # histogram:
    #geom_histogram(binwidth = 0.1) + 
    #geom_hline(0.1) +
}

weight_histogram <- function(edges, N, facet = TRUE){
  # N: number of observations
  
  get_p_value <- function(w, degrees) pchisq(2 * N * w, degrees, lower.tail = TRUE)
  w <- attr(edges, "weights")
  dfs <- attr(edges, "df")
  pvals <- unlist(Map(get_p_value, w, dfs))
  
  xx <- data.frame(index = (1:length(pvals))/length(pvals),
                   p = pvals,
                   df = factor(dfs))
  
  p <- ggplot(aes(x = p, col = df), data = xx) +
    geom_histogram(binwidth = 0.1)
  if (facet)
    p <- p + facet_wrap(~df)
  p
}
