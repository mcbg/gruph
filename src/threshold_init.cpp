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

/**
 * Calculates and sorts edges s.t. 
 */

 // [[Rcpp::export]]
NumericMatrix cont_threshold_init(NumericMatrix m, double lambda)
{
  // step 0: initate
  threshold_edge<gaussian_mutual_information> te;
  te.lambda = lambda;
  
  // step 1: calculate and sort edges
  std::vector<w_edge> edges = te(m);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // convert to R matrix
  int n = edges.size();
  NumericMatrix out(n, 2);
  NumericVector weights(n);
  
  for (int i = 0; i < out.nrow(); ++i) {
    weights[i] = edges[i].weight;
    out(i, 0) = edges[i].coord.first + 1;
    out(i, 1) = edges[i].coord.second + 1;
  }
  out.attr("weights") = weights;
  
  return out;
}
  