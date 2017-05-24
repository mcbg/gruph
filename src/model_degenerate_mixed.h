#ifndef DEGEN_MIX_MDL
#define DEGEN_MIX_MDL

#include "model.h"
#include "model_multivariate.h"
#include <unordered_map>

// [[Rcpp::plugins(cpp11)]]
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
  const int k = means.size();
  double var{0};

  // calculate SSD for each sample
  for(int i = 0; i < x.size(); ++i) {
    ssd[d[i]] += (x[i] - means[d[i]]) * (x[i] - means[d[i]]);
  }
  
  for(auto &p : ssd) {
    var += p.second;
    //p.second /= counts[p.first]; // we normalize using N_j, so we use the ML estimate
  }
  var /= x.size();
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
  var /= non_zero_size;
  return var;
}

class gaussian_degenerate_zero_mixed : public model
{
  /**
   * This uses Steffens approximation
   */
public:
  multivariate mult;
  gaussian_degenerate_zero_mixed(double l) : model(l), mult(l) {} 
  
  /**
   * param u: continous variable with degenerate zero
   * param d: discrete variable
   * 
   * changes df
   * returns: mutual information between variables
   */
  double mutual_information(variable u, variable d)
  {
   double sigma{ variance_conditioned(u, d) };
   double sigma_tilde{ variance_skip_zero(u) };
   double information{0};
   variable a{make_expression_indicator(u)};
   
   df = 0; // reset df!
   
   // calculate probabilites
   double probability_nonzero{0};
   for(auto x : a) { probability_nonzero += x; }
   probability_nonzero /= a.size();
  
   // calculate I(A,D)
   information += mult.mutual_information(a, d);
   df += mult.get_df();
   
   // second term
   
   information += probability_nonzero * 0.5 * log(sigma_tilde / sigma);
   df += 1;
   
   return information;
  }
  
};

#endif
