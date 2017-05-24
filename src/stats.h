/**
 * Useful functions for statistics
 *
 */
// [[Rcpp::plugins(cpp11)]]
#include <algorithm>
#include <cmath>
#include <Rcpp.h>
#include <unordered_map>

#ifndef STATS_H
#define STATS_H

typedef std::vector<double> vec;
typedef std::pair<vec::iterator, vec::iterator> pairit;

template<typename T>
class stats_functions 
{
public:
  double mean(const T &v)
  {
    double mean = std::accumulate(v.begin(), v.end(), 0.0);
    return mean /= v.size() ;
  }
  
  double cor(T x, T y)
  {
    normalize(x);
    normalize(y);
    
    // calculate constant k
    double k = x.size() - 1;
    k *=  std::sqrt(var_c(x));
    k *=  std::sqrt(var_c(y));
    
    double correlation = 0;
    
    for(pairit it(x.begin(), y.begin());
        it.first != x.end();
        ++it.first, ++it.second) 
      correlation += *it.first *  *it.second;
  
    correlation /= k;
    return correlation;
  }

  double var_c(const T &v) // central
  {
    double mu = mean(v);
    auto f = [mu] (double oursum, double x) {
      return oursum + (x - mu) * (x - mu) ;
    };
    double var = std::accumulate(v.begin(), v.end(), 0.0, f);
    int k = v.size() - 1; // ml
    
    return (var / k) ;
  }

  
  double var(const T &v) // ml
  {
    double mu = mean(v);
    auto f = [mu] (double oursum, double x) {
      return oursum + (x - mu) * (x - mu) ;
    };
    double var = std::accumulate(v.begin(), v.end(), 0.0, f);
    int k = v.size(); // ml
    
    return (var / k) ;
  }
  

  double sd(const T &v) // maximum likelihood??
  {
    return std::sqrt(var(v));
  }
  
  void normalize(T &x)
  { 
    double m  = mean(x);
  
    for(auto &num : x)
      num -= m;
  }
};

template<typename V>
class testing
{
public:
  double anova_var(const V &x, const V &y, size_t *df)
  {
    std::unordered_map<double, double> means;
    std::unordered_map<double, unsigned int> counts;
    std::unordered_map<double, double> sample_ssd;
    
    // 1) calc sums for means and counts
    for(int i = 0; i < x.size(); ++i) {
      means[y[i]] += x[i];
      ++counts[y[i]];
    }
    
    // 2) calc means from sums
    for (auto &p : means) {
      p.second /= counts[p.first];
    }
    
    // 3) calculate N * ssd for each sample
    for(int i = 0; i < x.size(); ++i) {
      sample_ssd[y[i]] += (x[i] - means[y[i]]) * (x[i] - means[y[i]]);
    }
    
    // 4) calculate ssd
    double ssd = 0;
    for (const auto &t : sample_ssd) {
      // theres no need to times and then divide with the
      ssd += t.second;
    }
    ssd /= x.size(); // TODO: SHOULD BE N - k ??
    
    // 5) set df
    *df = means.size() - 1;
    
    return ssd;
  }
  
  double anova_chisq(V x, V y, size_t *df) 
  {
    // step 1) calculate sums and counts
    stats_functions<V> sf;
    
    
    // step 2) calc. ssd0
    double ssd0 = sf.var(x) ;
    
    // step 3) calc ssd
    double ssd = anova_var(x, y, df); // changes df
    
    return 0.5 * log(ssd0 / ssd);
 }
};
#endif
