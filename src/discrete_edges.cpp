// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
using namespace Rcpp;

typedef std::vector<std::string> string_vec;

class binomial_mutual_information
{
  std::set<std::string> get_labels(string_vec);
  
public:  
  double operator()(string_vec, string_vec);
};

std::set<std::string> binomial_mutual_information::get_labels(string_vec x)
{
  std::set<std::string> setx(x.begin(), x.end());
  return setx;
}

double binomial_mutual_information::operator()(string_vec x,
                                     string_vec y)
{
  std::set<std::string> xlabels = get_labels(x);
  std::set<std::string> ylabels = get_labels(y);
  
  double chi_sq_coef = 0;
  
  // this can be optimised with maps
  for(auto xl : xlabels) {
    for(auto yl : ylabels) {
      double obs = 0;
      double xobs = 0;
      double yobs = 0;
      
      for (int i = 0; i < x.size(); ++i) {
        if (y[i] == yl && x[i] == xl)
            ++obs;
        if (y[i] == yl)
            ++yobs;
        if (x[i] == xl)
          ++xobs;
      }
      
      double expected = (xobs * yobs) / x.size();
      std::cout << obs << std::endl;
      chi_sq_coef += std::pow(obs - expected, 2) / expected;
    }
  }
  double df = (xlabels.size() - 1) * (ylabels.size() - 1);
  // lower tail: false, log: false
  return R::pchisq(chi_sq_coef, df, false, false);
}

// [[Rcpp::export]]
double hi(StringVector x, StringVector y) 
{
  string_vec xs = as<string_vec>(x);
  string_vec ys = as<string_vec>(y);
  
  return binomial_mutual_information()(xs, ys);
}

NumericMatrix discrete_edges(const CharacterMatrix xx)
{
  CharacterVector x = xx(_, 0); // smarter way to do this..
  CharacterVector y = xx(_, 0);
  
  for (int i = 0; i < xx.ncol(); ++i) {
    x = xx(_, i);
    for (int j = i + 1; j < xx.ncol(); ++j) {
      y = xx(_, j);
    
    }
  }
  return NumericMatrix(4, 2);
}
/*** R
microbenchmark::microbenchmark(
  hi(mtcars$cyl, mtcars$gear),
  chisq.test(mtcars$cyl, mtcars$gear)
)
*/