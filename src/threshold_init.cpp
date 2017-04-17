#include<Rcpp.h>

#include "w_edge.h"
#include "forest.h"
#include "model_gaussian.h"
#include "model_multivariate.h"
#include "threshold_initializer.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]


 // [[Rcpp::export]]
NumericMatrix cont_threshold_init(NumericMatrix m, double lambda)
{
  // step 0: initate
  threshold_initializer<gaussian> init(lambda);
  
  // step 1: calculate and sort edges
  std::vector<w_edge> edges = init.initialize(m);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // convert to R matrix, TODO: move this to w_edge
  
  int n = edges.size();
  NumericMatrix out(n, 2);
  NumericVector weights(n);
  NumericVector df(n);
  
  for (int i = 0; i < n; ++i) {
    weights[i] = edges[i].weight;
    df[i] = edges[i].df;
    
    out(i, 0) = edges[i].i + 1;
    out(i, 1) = edges[i].j + 1;
  }
  
  out.attr("weights") = weights;
  out.attr("df") = df;
  
  return out;
}
  
