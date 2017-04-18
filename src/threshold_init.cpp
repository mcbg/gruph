#include<Rcpp.h>

#include "w_edge.h"
#include "forest.h"
#include "model_gaussian.h"
#include "model_multivariate.h"
#include "threshold_initializer.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

void wrap_edges(const std::vector<w_edge> edges, NumericMatrix *out)
{
  /** 
   * Populates `out` with the contents of `edges`.
   */
  
  int n = edges.size();
  NumericVector weights(n);
  NumericVector df(n);
  
  for (int i = 0; i < n; ++i) {
    weights[i] = edges[i].weight;
    df[i] = edges[i].df;
    
    (*out)(i, 0) = edges[i].i + 1;
    (*out)(i, 1) = edges[i].j + 1;
  }
  
  out->attr("weights") = weights;
  out->attr("df") = df;
}

template<class M>
NumericMatrix threshold_init(const NumericMatrix m, const double lambda)
{
  // step 0: initate
  threshold_initializer<M> init(lambda);
  
  // step 1: calculate and sort edges
  std::vector<w_edge> edges = init.initialize(m);
  
  // step 2: convert to R matrix
  NumericMatrix out(edges.size(), 2);
  wrap_edges(edges, &out);
  
  return out;
}

 // [[Rcpp::export]]
NumericMatrix cont_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<gaussian>(m, lambda);
}
  
 // [[Rcpp::export]]
NumericMatrix discrete_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<multivariate>(m, lambda);
}
