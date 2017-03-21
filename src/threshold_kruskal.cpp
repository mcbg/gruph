#include<Rcpp.h>

#include "w_edge.h"
#include "forest.h"
#include "gaussian_mutual_information.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::plugins(cpp14)]]

template<class mutual_info>
struct threshold_edge {
  double lambda;
  std::vector<w_edge> operator()(NumericMatrix data)
  {
    std::vector<w_edge> q; // TODO: Initialize this proper
    
    for(int i = 0; i < data.ncol(); ++i) {
      NumericVector x = data(_, i);
      for(int j = i + 1; j < data.ncol(); ++j) {
        NumericVector y = data(_, j);
        w_edge mEdge = {mutual_info()(x, y), {i, j}};
        if (mEdge.weight > lambda)
          q.push_back(mEdge);
      }
    }
    return q;
  }
};

// [[Rcpp::export]]
NumericMatrix threshold_kruskal(NumericMatrix m, double lambda)
{
  // step 1: sort edges
  
  threshold_edge<gaussian_mutual_information> te;
  te.lambda = lambda;
  std::vector<w_edge> edges = te(m);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // step 2: Add edges to forest---edges that induce cycles are rejected.
  forest f(edges);
  
  return f.get_edges();
}
  