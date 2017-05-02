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
// [[Rcpp::depends(BH)]]

void wrap_edges(const std::vector<w_edge> edges,
                const CharacterVector colnames,
                NumericMatrix *out)
{
  /** 
   * Populates `out` with the contents of `edges`.
   */
  
  int n = edges.size();
  NumericVector weights(n);
  NumericVector df(n);
  
  auto node_style = [edges, colnames] (int i, bool is_first) {
    int k = is_first ? edges[i].i : edges[i].j;
    //return colnames[k];
    return k++;
  };
  
  boost::progress_display loading_bar(n);
  for (int i = 0; i < n; ++i) {
    weights[i] = edges[i].weight;
    df[i] = edges[i].df;
    
    
    (*out)(i, 0) = node_style(i, true);
    (*out)(i, 1) = node_style(i, false);
    
    ++loading_bar;
  }
  
  out->attr("weights") = weights;
  out->attr("df") = df;
  out->attr("columns") = colnames;
}

template<class M>
NumericMatrix threshold_init(const NumericMatrix &x,
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
  NumericMatrix out(edges.size(), 2);
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
NumericMatrix cont_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<gaussian>(m, lambda);
}
  
 // [[Rcpp::export]]
NumericMatrix discrete_threshold_init(NumericMatrix m, double lambda)
{
  return threshold_init<multivariate>(m, lambda);
}


 // [[Rcpp::export]]
NumericMatrix mixed_threshold_init(NumericMatrix x,
                                        NumericMatrix y,
                                        double lambda)
{
  const int index_offset  = x.ncol(); // size of x
  if (x.nrow() != y.nrow())
    stop("Rows don't match!");

  const CharacterVector xNames = VECTOR_ELT(x.attr("dimnames"), 1);
  const CharacterVector yNames = VECTOR_ELT(y.attr("dimnames"), 1);
  const CharacterVector newNames = concat(as<strvec>(xNames), as<strvec>(yNames));
  
  threshold_initializer builder(lambda);
  std::vector<w_edge> edges;
  
  // step 1) calculate edges
  gaussian gauss(lambda);
  multivariate disc(lambda);
  mixed mix(lambda);
  
  Rcout << "Calculating gaussian edges.." << std::endl;
  builder.add_edges(x, 0, &gauss, &edges);
  Rcout << "\nCalculating discrete edges.." << std::endl;
  builder.add_edges(y, index_offset, &disc, &edges);
  Rcout << "\nCalculating mixed edges.." << std::endl;
  builder.add_edges_mixed(x, y, 0, index_offset, &mix, &edges);
  
  // step 2) sort
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  // step 3) make r friendly
  NumericMatrix out(edges.size(), 2);
  
  Rcout << "\nConverting to R objects.." << std::endl;
  wrap_edges(edges, newNames, &out);
  
  return out;
}
