# graph plotting
# this is an issue: https://stackoverflow.com/questions/9888619/plot-of-undirected-igraph-shows-gaps-between-edge-and-vertex-on-one-end-of-edge

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
  g <- graph_from_edgelist(myedges[, 1:2], directed = FALSE)
  V(g)$names <- vnames
  g
}

#' \code{plot_neighbourhood} makes a igraph object with labelled vertices.
#' 
#' @import igraph
#' @export
plot_neighbourhood <- function(g, proband, jumps) {
  gn <- make_ego_graph(g, jumps, proband)[[1]]
  plot(gn)
}
