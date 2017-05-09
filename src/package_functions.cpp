#include<Rcpp.h>

// MODELS
#include "model_gaussian.h"
#include "model_multivariate.h"
#include "model_mixed.h"
#include "model_gaussian_degenerate_zero.h"

#include "w_edge.h"
#include "wrapper.h"
#include "threshold_initializer.h"
#include "stats.h"

using namespace Rcpp;

// TODO: change names to cinit dinit minit

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(BH)]]


template<class M>
NumericMatrix threshold_init(const NumericMatrix &x,
                               const double lambda)
{
  // step 0: initate initializer and colnames
  
  std::vector<w_edge> edges; 
  threshold_initializer init(lambda);
  M mModel = M(lambda);
  
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
  Rcout << "Calculating edges.." << std::endl;
  init.add_edges(x, 0, &mModel, &edges);
  
  // step 2: convert to R matrix
  Rcout << "Converting to R object.." << std::endl;
  NumericMatrix out(edges.size(), 2);
  wrapper w;
  w(edges, xNames, &out);
  
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
double hi(NumericVector x, NumericVector y)
{
  gaussian_degenerate_zero mdl(0);
  return mdl.mutual_information(x, y);
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
  wrapper w;
  w(edges, newNames, &out);
  
  return out;
}
