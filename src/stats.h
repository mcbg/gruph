/**
 * Useful functions for statistics
 *
 */
// [[Rcpp::plugins(cpp11)]]
#include <algorithm>
#include <cmath>

#ifndef STATS_H
#define STATS_H

template<typename T>
class stats_functions 
{
public:
  double mean(const T &v)
  {
    double mean = std::accumulate(v.begin(), v.end(), 0.0);
    return mean /= v.size();
  }
  
  double sd(const T &v)
  {
    double mu = mean(v);
    auto f = [mu] (double x) { return (x - mu) * (x - mu) ;};
    double var = std::accumulate(v.begin(), v.end(), 0.0, f);
    return std::sqrt(var / v.size());
  }
  
  void normalize(T &x)
  { 
    double m  = mean(x);
  
    for(auto &num : x)
      num -= m;
  }
};

#endif