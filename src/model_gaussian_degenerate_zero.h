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

constexpr int isNotZero (double x) { return x != 0; };
inline double gaussian_mutual (double rho) { return -log(1 - rho * rho); };

class gaussian_degenerate_zero : public model
{
  multivariate mult;
  double local_cor(const variable&, const variable&);
public:  
  gaussian_degenerate_zero(double l) : model(l), mult(l) { } 
  double mutual_information(variable, variable);
};

double gaussian_degenerate_zero::local_cor(const variable &x, const variable &y)
{
  vector<double> mx;
  vector<double> my;
  mx.reserve(x.size());
  my.reserve(x.size());
  
  for(auto xit = begin(x), yit = begin(y);
      xit != end(x); ++xit, ++yit) {
    if (*xit && *yit) {
      mx.push_back(*xit);
      my.push_back(*yit);
    }
  }
  return sf.cor(mx, my);
}

double gaussian_degenerate_zero::mutual_information(variable x, variable y)
{
  double information{0};
  
  // run checks
  if (x.size() != y.size())
    stop("dimension problem");
    
  // calculate probabilities of (A,B)
  double p[2][2]{0}; // probabilities
  for (int i = 0; i < x.size(); ++i) 
    ++p[ isNotZero(x[i]) ][ isNotZero(y[i]) ];
    
  for_each(begin(p), end(p),
    [&y] (double x[2]) {
      x[0] /= y.size();
      x[1] /= y.size();
  });
  
  // calculate I(A,B)
  variable a(y.size());
  variable b(y.size());
  transform(begin(x), end(x), begin(a), isNotZero);
  transform(begin(y), end(y), begin(b), isNotZero);
  if(p[1][1] != 1)
    information += mult.mutual_information(a, b);
  
  // calculate I(X,Y)
  if (p[1][1])
    information += p[1][1] * gaussian_mutual(this->local_cor(x,y));
    
  return information;
}
#endif