// [[Rcpp::plugins(cpp11)]]

#include "model_gaussian.h"
#include "stats.h"
#include<vector>

double gaussian::mutual_information(variable px, variable py)
{
  
  // convert to std vectors
  vec x = as<vec>(px);
  vec y = as<vec>(py);
  
  // calculate info
  stats_functions<vec> stats;
  double correlation = stats.cor(x, y);
  
  return -0.5 * x.size() * std::log(1 - correlation * correlation);
}
