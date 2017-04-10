// [[Rcpp::plugins(cpp11)]]

#include "model.h"
#include <Rcpp.h>
#include <unsorted_map>
#include <cmath>

class mixed : model
{
  double mutual_information(variable, variable);
};

double model::mutual_information(variable x, variable y)
{
  // CALCULATE INFO
  int n = x.size();
  double total_mean = 0;
  double s0 = 0; // ungrouped
  double s = 0; // grouped
  std::unordered_map<int, double> group_mean;
  std::unordered_map<int, double> group_count;
  
  // sum continous variables and count group elements
  for (int i = 0; i < n; ++i) {
    group_mean[y[i]] += x[i];
    total_mean += x[i];
    
    ++group_count[y[i]];
  }
  
  // SET DF
  df = group_mean.size() - 1; // TODO: double check
    
  // divide with thingy
  total_mean /= n;
  
  for(auto p : group_mean) {
    auto key = p.first;
    group_mean[key] /= group_count[key];
  }

  for (int i = 0; i < n; ++i) {
    s0 += std::pow(x[i] - total_mean, 2);
    s += std::pow(x[i] - group_mean[y[i]], 2);
  }
  s0 /= n;
  s /= n;
  
  // TODO: MI from Edwards et al. (??)
  // Remove include n?
  return log(s0/s) / 2;
}
