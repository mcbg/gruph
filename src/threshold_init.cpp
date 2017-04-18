#include<Rcpp.h>

#include "w_edge.h"
#include "forest.h" // not used?
#include "model_gaussian.h"
#include "model_multivariate.h"
#include "threshold_initializer.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

void wrap_edges(const std::vector<w_edge> edges,
                const CharacterVector colnames,
                CharacterMatrix *out)
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
    
    (*out)(i, 0) = colnames[edges[i].i];
    (*out)(i, 1) = colnames[edges[i].j];
  }
  
  out->attr("weights") = weights;
  out->attr("df") = df;
}

template<class M>
CharacterMatrix threshold_init(const NumericMatrix m, const double lambda)
{
  // step 0: initate initializer and colnames
  threshold_initializer<M> init(lambda);
  CharacterVector colnames = VECTOR_ELT(m.attr("dimnames"), 1);
  
  // step 1: calculate and sort edges
  std::vector<w_edge> edges = init.initialize(m);
  
  // step 2: convert to R matrix
  CharacterMatrix out(edges.size(), 2);
  wrap_edges(edges, colnames, &out);
  
  return out;
}

 // [[Rcpp::export]]
CharacterMatrix cont_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<gaussian>(m, lambda);
}
  
 // [[Rcpp::export]]
CharacterMatrix discrete_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<multivariate>(m, lambda);
}
