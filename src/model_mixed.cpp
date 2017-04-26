#include "model_mixed.h"

double mixed::mutual_information(variable x, variable y)
{
  // step 1) calculate sums and counts
  std::unordered_map<double, int> means;
  std::unordered_map<double, double> counts;
  
  for(int i = 0; i < x.size(); ++i) {
    means[y[i]] += x[i];
    ++counts[y[i]];
  }
  
  // step 2) calculate means
  double mean = 0;
  
  for (auto &p : means) {
    mean += p.second;
    p.second /= counts[p.first];
  }
  
  mean /= x.size();
  
  // step 3) calculate s_0
  
  auto f = [mean] (double ourSum, double x) {
      return ourSum + (x - mean) * (x - mean) ;
  };
  
  double s0 = std::accumulate(x.begin(), x.end(), 0.0, f);
  
  // step 4) calculate s
  
  double s = 0;
  
  for (int i = 0; i < x.size(); ++i) {
    double term = x[i] - means[y[i]];
    s += term * term;
  }
  
  s /= x.size();
  
  // step 5) calc df and mutual information
  df = means.size() - 1;
  
  return 0.5 * log(s0 / s);
}