// [[Rcpp::plugins(cpp11)]]

#ifndef DEGEN_MDL
#define DEGEN_MDL

#include "stats.h"
#include "model.h"
#include "model_multivariate.h"
#include <cmath>
#include <Rcpp.h>
//#include <boost/iterator/zip_iterator.hpp>

using namespace Rcpp;
using namespace std;

typedef std::vector<double> vec;
stats_functions<vec> sf;

template<typename T>
constexpr T isNotZero (double x) 
{ 
  return x != 0 ? T(1) : T(0);
};

inline double gaussian_mutual (double rho) { return - log(1 - rho * rho) / 2; };

class gaussian_degenerate_zero : public model
{
  multivariate mult;
  double local_cor(const variable&, const variable&);
public:  
  gaussian_degenerate_zero(double l) : model(l), mult(l) {
  } 
  double mutual_information(variable, variable);
};

double gaussian_degenerate_zero::local_cor(const variable &x, const variable &y)
{
  vector<double> mx;
  vector<double> my;
  //mx.reserve(x.size());
  //my.reserve(x.size());
  
  for(auto xit = begin(x), yit = begin(y);
      xit != end(x); ++xit, ++yit) {
    if (*xit != 0.0 && *yit != 0.0) {
      mx.push_back(*xit);
      my.push_back(*yit);
    }
  }
  return sf.cor(mx, my);
}

double gaussian_degenerate_zero::mutual_information(variable x, variable y)
{
  double information{0};
  df = 0; // reset df
  
  // run checks
  if (x.size() != y.size())
    stop("dimension problem");
    
  // calculate probabilities of (A,B)
  double p[2][2]{0}; // probabilities
  for (int i = 0; i < x.size(); ++i) 
    ++p[ isNotZero<int>(x[i]) ][ isNotZero<int>(y[i]) ];
  
  /** 
   * p hasn't been normalized yet, so it just has the counts of each case.
   * We only include the gaussian term if there are 3 or more entries.
   */
  const bool include_gaussian_term = p[1][1] >= 3;
    
  for_each(begin(p), end(p),
    [&y] (double x[2]) {
      x[0] /= y.size();
      x[1] /= y.size();
  });
  
  // calculate I(A,B)
  if(p[1][1] != 1) {
    variable a(y.size());
    variable b(y.size());
    transform(begin(x), end(x), begin(a), isNotZero<double>);
    transform(begin(y), end(y), begin(b), isNotZero<double>);
    information += mult.mutual_information(a, b);
    df += mult.get_df();
  }
  
  // calculate I(X,Y)
  if (include_gaussian_term) {
    double rho = this->local_cor(x,y);
    information += p[1][1] * gaussian_mutual(rho);
    df += 1;
  }
    
  return information;
}
#endif