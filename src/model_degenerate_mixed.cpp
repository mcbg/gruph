#include "model_degenerate_mixed.h"

/**
 * TODO: rename VAR to SSD throughout
 */

variable make_expression_indicator(const variable &x)
{
  // Returns vector that indicates 
  variable a(x.size());
  std::transform(x.begin(), x.end(), a.begin(), [] (double x) { return x != 0 ? 1.0 : 0.0; });
  return a;
} 

double variance_conditioned(variable x, variable d)
{
  std::unordered_map<double, double> means;
  std::unordered_map<double, double> counts;
  std::unordered_map<double, double> ssd;
  
  unsigned int non_zero_size{0};

  // CALCULATE "MEANS"
  for (int i = 0; i < x.size(); ++i) {
    means[d[i]] += x[i];
    if (x[i] != 0)
      ++counts[d[i]];
  }

  for(auto &p : means) {
    p.second /= counts[p.first];
  }

  // calculate variance
  double var{0};

  // calculate SSD for each group
  for(int i = 0; i < x.size(); ++i) {
    ssd[d[i]] += (x[i] - means[d[i]]) * (x[i] - means[d[i]]);
  }
  
  for(auto &p : ssd) {
    var += p.second;
    //p.second /= counts[p.first]; // we normalize using N_j, so we use the ML estimate
  }
  
  for(auto &p : counts) 
    non_zero_size += p.second;
  
  //var /= non_zero_size;
  return var;
}

double variance_skip_zero(variable x)
{
  double mean{0};
  double var{0};
  int non_zero_size{0};

  // CALCULATE "MEAN"
  for(auto e : x) {
    mean += e;

    if (e != 0)
      ++non_zero_size;
  }

  mean /= non_zero_size;

  // CALCULATE VARIANCE
  for(auto e : x) {
    var += (e - mean) * (e - mean);
  }
  //var /= non_zero_size;
  return var;
}

double gaussian_degenerate_zero_mixed::mutual_information(variable u, variable d)
{
  size_t n = u.size();
  double sigma{ variance_conditioned(u, d) };
  double sigma_tilde{ variance_skip_zero(u) };
  double information{0};
  variable a{make_expression_indicator(u)};
 
  df = 0; // reset df!
 
  // calculate probabilites
  double probability_nonzero{0};
  for(auto x : a) { probability_nonzero += x; }
  probability_nonzero /= n;

  // calculate I(A,D)
  if (probability_nonzero != 1) {
    information += mult.mutual_information(a, d);
    df += mult.get_df();
  }
 
  // second term
  if (probability_nonzero != 0 &&
      sigma_tilde > 0 &&
      sigma > 0)
    information += probability_nonzero * n * 0.5 * log(sigma_tilde / sigma);
  
  df += 1; 
 
  return information;
}
