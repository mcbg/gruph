#include<Rcpp.h>

// MODELS
#include "model_gaussian.h"
#include "model_multivariate.h"
#include "model_mixed.h"

#include "w_edge.h"
#include "threshold_initializer.h"
#include "stats.h"

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
    
    auto node_style = [edges, colnames] (int i, bool is_first) {
      int k = is_first ? edges[i].i : edges[i].j;
      return colnames[k];
    };
    
    (*out)(i, 0) = node_style(i, true);
    (*out)(i, 1) = node_style(i, false);
  }
  
  out->attr("weights") = weights;
  out->attr("df") = df;
  out->attr("columns") = colnames;
}

template<class M>
CharacterMatrix threshold_init(const NumericMatrix &x,
                               const double lambda)
{
  // step 0: initate initializer and colnames
  
  std::vector<w_edge> edges; 
  threshold_initializer init(lambda);
  model *mModel = new M(lambda);
  
  auto gen_colnames = [] (int n) {
    CharacterVector x(n);
    int i = 1;
    std::generate(x.begin(), x.end(), [&i]() { return i++; });
    return x;
  };
  
  SEXP dimnames = x.attr("dimnames");
  CharacterVector xNames = !Rf_isNull(dimnames) ?
    CharacterVector(VECTOR_ELT(x.attr("dimnames"), 1)) :
      gen_colnames(x.ncol());
  
  // step 1: calculate and sort edges
  init.add_edges(x, 0, mModel, &edges);
  
  // step 2: sort
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // step 3: convert to R matrix
  CharacterMatrix out(edges.size(), 2);
  wrap_edges(edges, xNames, &out);
  
  return out;
}
  
typedef std::vector<std::string> strvec;
CharacterVector concat(strvec x, strvec y)
{
  strvec z(x.size() + y.size());
  std::copy(x.begin(), x.end(), z.begin());
  std::copy(y.begin(), y.end(), z.begin() + x.size());
  return wrap(z);
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


 // [[Rcpp::export]]
CharacterMatrix mixed_threshold_init(NumericMatrix x,
                                        NumericMatrix y,
                                        double lambda)
{
  const int index_offset  = x.ncol(); // size of x
  
  const CharacterVector xNames = VECTOR_ELT(x.attr("dimnames"), 1);
  const CharacterVector yNames = VECTOR_ELT(y.attr("dimnames"), 1);
  const CharacterVector newNames = concat(as<strvec>(xNames), as<strvec>(yNames));
  threshold_initializer builder(lambda);
  std::vector<w_edge> edges;
  
  // step 1) calculate edges
  gaussian gauss(lambda);
  multivariate disc(lambda);
  mixed mix(lambda);
  
  builder.add_edges(x, 0, &gauss, &edges);
  builder.add_edges(y, index_offset, &disc, &edges);
  builder.add_edges_mixed(x, y, 0, index_offset, &mix, &edges);
  
  // step 2) sort
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // step 3) make r friendly
  CharacterMatrix out(edges.size(), 2);
  wrap_edges(edges, newNames, &out);
  
  return out;
}