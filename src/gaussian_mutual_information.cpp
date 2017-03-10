#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <queue>

using namespace Rcpp;

// [[Rcpp::plugins(cpp14)]]
// [[Rcpp::plugins(cpp11)]]

typedef std::vector<double> vecdub;
typedef std::pair<vecdub::iterator, vecdub::iterator> pairit;


class gaussian_mutual_information
{
  double mean(vecdub x)
  {
    double ourmean = 0;
    
    for(auto num : x) 
      ourmean += num / x.size();
    
    return ourmean;
  }
  
  void normalize(vecdub &x)
  { 
    double m  = mean(x);
    
    for(auto &num : x)
      num -= m;
  }
  
  double sd(vecdub x)
  {
    double mysd = 0;
    normalize(x);
    
    for(double num : x)
      mysd += std::pow(num, 2);
    
    mysd /= x.size() - 1;
    return std::sqrt(mysd);
  }
  
public:  
  double operator()(NumericVector x,
                                     NumericVector y) {
    vecdub xn = as<vecdub>(x);
    vecdub yn = as<vecdub>(y);
    normalize(xn);
    normalize(yn);
    
    double k = x.size() - 1;
    k *= sd(xn);
    k *= sd(yn);
    
    double correlation = 0;
    
    for(pairit it(xn.begin(), yn.begin()) ; it.first != xn.end(); ++it.first, ++it.second) {
      correlation += *it.first *  *it.second;
    }
    
    correlation /= k;
    return -1 * std::log(1 - correlation * correlation);
  }
};
