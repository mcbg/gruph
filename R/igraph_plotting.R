# graph plotting

igraph_style <- function(g) {
  V(g)$size <- 4
  V(g)$color <- V(g)$frame.color <- "gray70"
  
  V(g)$label.cex <- 0.8
  V(g)$label.color <- "black"
  g
}

#' \code{glasso_plot} adds edges to our tree, that are computed
#' using the glasso algorithm. These makes most sense for vertices
#' that are roughly Gaussian.
#' @param rho the penalty of the glasso
#' @param tree a igraph object of our tree
#' @param dataset our dataset to fit the glasso
#' 
#' @import glasso
#' @import igraph
#' @export
glasso_plot <- function(tree, glasso) {
  
  #ISSUE: ??????????????????
  stopifnot(all(V(tree)$name == V(glasso)$name))
  
  g <- glasso + edges(t(as_edgelist(tree)), color = 'gray30')
  #g <- union(tree, glasso, byname = 'auto')
  #g <- style_graph(g)
  #g <- gene_fam_style(g)
  
  #  stopifnot(all(V(tree)$name == V(g)$name))
  V(g)$name <- sub('^ENS.*','', V(g)$name) #sub('^ENS.*', '', V(g)$name)
  
  set.seed(123)
  plot(g, layout = layout.fruchterman.reingold(tree), edge.arrow.mode = '-') 
}

#' \code{make_igraph} makes a igraph object with labelled vertices.
#' 
#' @param edges edge list of maximum spanning forest.
#' @param dataset list of two matrices used for fitting edges
#' 
#' @import igraph
#' @export
make_igraph <- function(myedges, dataset) {
  # ISSUE: lazy hardcoded
  vnames <- c(colnames(dataset$expression), "mutation")
  g <- graph_from_edgelist(as.matrix(myedges[, 1:2]), directed = FALSE)
  V(g)$name <- vnames
  g <- igraph_style(g)
  g
}

#' \code{plot_neighbourhood} makes a igraph object with labelled vertices.
#' 
#' @import igraph
#' @export
plot_neighbourhood <- function(g, proband, jumps) {
  gn <- make_ego_graph(g, jumps, proband)[[1]]
  
  # EDGE COLOURS
  E(gn)$color <- "gray60"
  E(gn)$color[E(gn)$weight > 20] <- "lightpink"
  E(gn)$color[E(gn)$weight > 45] <- "deeppink"
  
  # VERTEX COLOURS
  V(gn)$color[V(gn)$name == proband] <- "steelblue1"
  V(gn)$frame.color[V(gn)$name == proband] <- "steelblue1"
  
  par(mar = c(0,0,0,0)) # get rid og margins
  set.seed(123)
  l <- layout.fruchterman.reingold(gn)
  plot(gn, layout = l)
}

bonferroni_edges <- function(edges, logN) {
  # N vertices means N(N-1)/2 tests, which log-transformed is 
  # approximately this
  logM <- 2*logN - log(2) 
  
  ws <- edges$weights
  dfs <- edges$df
  
  f <- function(w, df) {
    x <- pchisq(2 * w, df, lower.tail = FALSE, log.p = TRUE)
    exp(x + logM)
  }
  
  x <- unlist(Map(f, ws, dfs))
  x
}

#' \code{plot_neighbourhood_bonferroni} makes a igraph object with labelled vertices.
#' Edges are coloured w.r.t. the Bonferroni correction with \eqn{\alpha = 0.05}.
#' 
#' @param g a igraph object
#' @param proband name of vertex to center neighbourhood around
#' @param jumps number of jumps to include in neighbourhood
#' @param edges gruph edge list
#' 
#' @import igraph
#' @export
plot_neighbourhood_bonferroni <- function(g, proband, jumps, edges) {
  
  # BONFERRONI EDGES
  N <- length(V(g))
  bon_sig <- bonferroni_edges(edges, log(N)) < 0.05
  E(g)$color <- ifelse(bon_sig, 'deeppink', 'gray70')
  
  # VERTEX COLOURS
  V(g)$color[V(g)$name == proband] <- "steelblue1"
  V(g)$frame.color[V(g)$name == proband] <- "steelblue1"
  
  gn <- make_ego_graph(g, jumps, proband)[[1]]
  p <- plot(gn)
  p
}

#' \code{mark_bonferroni_edges} makes a igraph object with labelled vertices.
#' Edges are coloured w.r.t. the Bonferroni correction with \eqn{\alpha = 0.05}.
#' This function must be used
#'
#' @param g a igraph object
#' @param edges gruph edge list
#'
#' @return a igraph object with pink Bonferroni edges
#'
#' @import igraph
#' @export
mark_bonferroni_edges <- function(g, edges) {
  
  # BONFERRONI EDGES
  N <- length(V(g))
  bon_sig <- bonferroni_edges(edges, log(N)) < 0.05
  E(g)$color <- ifelse(bon_sig, 'deeppink', 'gray70')
  
  return(g)
}
