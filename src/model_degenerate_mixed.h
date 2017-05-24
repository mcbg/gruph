#ifndef DEGEN_MIX_MDL
#define DEGEN_MIX_MDL

#include <unordered_map>

variable make_expression_indicator(const variable &x)
{
  // Returns vector that indicates 
  variable a(x.size());
  transform(begin(x), end(x), begin(a), isNotZero<double>);
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
      ++counts[d[i]]
  }

  for(auto &p : means) {
    p.second /= counts[p.first];
  }

  // calculate variance
  const int k = means.size();
  double var{0};

  // calculate SSD for each sample
  for(int i = 0; i < x.size(); ++i) {
    sample_ssd[d[i]] += (x[i] - means[y[i]]) * (x[i] - means[y[i]]);
  }
  
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
    var += (e - mean) * (e - mean)
  }
  var /= (non_zero_size - 1);
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
  
  double mutual_information(variable u, variable d)
  {
   double sigma_tilde;
   double sigma;
   double information{0};
   variable a{make_expression_indicator(u)};
   
   // calculate probabilites
   double probability_nonzero{0};
   for(auto x : a) { probability_nonzero += x; }
   probability_nonzero /= a.size();
  
   // calculate I(A,D)
   // TODO: condition on probability >0
   information += mult.mutual_information(a, d);
   
   // second term
   information += probability_nonzero * 0.5 * log(sigma_tilde / sigma);
  }
  
};

#endif
