/**
 * Useful functions for statistics
 *
 */
// [[Rcpp::plugins(cpp11)]]
#include <algorithm>
#include <cmath>

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
    k *= sd(x);
    k *= sd(y);
    
    double correlation = 0;
    
    for(pairit it(x.begin(), y.begin());
        it.first != x.end();
        ++it.first, ++it.second) 
      correlation += *it.first *  *it.second;
  
    correlation /= k;
    return correlation;
  }
  
  double sd(const T &v) // maximum likelihood??
  {
    double mu = mean(v);
    auto f = [mu] (double ourSum, double x) {
      return ourSum + (x - mu) * (x - mu) ;
    };
    double var = std::accumulate(v.begin(), v.end(), 0.0, f);
    return std::sqrt(var / (v.size() - 1));
  }
  
  void normalize(T &x)
  { 
    double m  = mean(x);
  
    for(auto &num : x)
      num -= m;
  }
};

#endif