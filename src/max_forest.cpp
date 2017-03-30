#include <Rcpp.h>
#include "forest.h"

using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix max_forest(NumericMatrix x, NumericVector w) {
  forest f(x, w);
  return f.get_edges();
}
