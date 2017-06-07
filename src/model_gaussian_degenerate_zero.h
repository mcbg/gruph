// [[Rcpp::plugins(cpp11)]]

#ifndef DEGEN_MDL
#define DEGEN_MDL

#include "stats.h"
#include "model.h"
#include "model_multivariate.h"
#include <cmath>
#include <Rcpp.h>

using namespace Rcpp;
using namespace std;

typedef std::vector<double> vec;

template<typename T>
constexpr T isNotZero (double x) 
{ 
  return x != 0 ? T(1) : T(0);
};

inline double gaussian_mutual (double rho) { return - log(1 - rho * rho) / 2; };

class gaussian_degenerate_zero : public model
{
  stats_functions<vec> sf;
  multivariate mult;
  double local_cor(const variable&, const variable&);
public:  
  gaussian_degenerate_zero(double l) : model(l), mult(l) {} 
  double mutual_information(variable, variable);
};


#endif
