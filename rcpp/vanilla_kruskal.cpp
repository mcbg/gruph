// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <algorithm>
#include <vector>

#include "gaussian_mutual_information.cpp"
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


struct w_edge_greater 
{
  bool operator() (const w_edge lhs, const w_edge rhs)
  {
    return std::abs(lhs.weight) >  std::abs(rhs.weight);
  }
};

// [[Rcpp::export]]
NumericMatrix vanilla_kruskal(NumericMatrix m)
{
  // step 1: sort edges
  std::vector<w_edge> edges = get_all_edges<gaussian_mutual_information>(m);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  forest f;
  
  for(auto e : edges) {
    std::cout << e.weight << ' ';
    f.add_edge(e);
    
  }  // wrong sort?
  
  return f.get_edges();
}

/*** R
library(igraph)
library(microbenchmark)

f <- function(n) vanilla_kruskal(matrix(rnorm(n * 880), ncol = n))

edges <- f(20)
class(edges) <- "character"
g <- graph_from_edgelist(edges)
plot(g)

*/ 