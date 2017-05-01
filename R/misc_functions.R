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

