// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <algorithm>
#include <vector>

#include "gaussian_mutual_information.h"
#include "forest.h"
#include "w_edge.h"

using namespace Rcpp;

template<class mutual_info>
std::vector<w_edge> get_all_edges(NumericMatrix data)
{
  std::vector<w_edge> q; // TODO: Initialize this proper
  
  for(int i = 0; i < data.ncol(); ++i) {
    NumericVector x = data(_, i);
    for(int j = i + 1; j < data.ncol(); ++j) {
      NumericVector y = data(_, j);
      w_edge mEdge = {mutual_info()(x, y), {i, j}};
      q.push_back(mEdge);
    }
  }
  return q;
}

// [[Rcpp::export]]
NumericMatrix vanilla_kruskal(NumericMatrix m)
{
  // step 1: sort edges
  std::vector<w_edge> edges = get_all_edges<gaussian_mutual_information>(m);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  forest f(edges);
  
  for(auto e : edges) {
    std::cout << e.weight << ' ';
    f.add_edge(e);
    
  }  // wrong sort?
  
  return f.get_edges();
}
  

/*** R
library(igraph)
library(microbenchmark)

N <- 100
df <-  matrix(rnorm(N * 880), ncol = N)
15*100/60/24
microbenchmark(
  vanilla_kruskal(df)
)


#class(edges) <- "character"
#g <- graph_from_edgelist(edges, directed = FALSE)
#plot.igraph(g,layout_as_tree(g), margin = -0.5,
             #vertex.label = NA, vertex.size = 2.0, edge.arrow.size = 22)
*/ 