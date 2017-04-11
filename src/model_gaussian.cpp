// [[Rcpp::plugins(cpp11)]]

#include "model_gaussian.h"
#include "stats.h"
#include<vector>

double gaussian::mutual_information(variable px, variable py)
{
  // set df
  df = 1;
  
  // convert to std vectors
  vec x = as<vec>(px);
  vec y = as<vec>(py);
  
  // calculate info
  stats_functions<vec> stats;
  
  stats.normalize(x);
  stats.normalize(y);
  
  // calculate constant k
  double k = x.size() - 1;
  k *= stats.sd(x);
  k *= stats.sd(y);
  
  double correlation = 0;
  
  for(pairit it(x.begin(), y.begin());
      it.first != x.end();
      ++it.first, ++it.second) 
    correlation += *it.first *  *it.second;

  correlation /= k;
  return -0.5 * std::log(1 - correlation * correlation);
}
