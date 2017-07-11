#include<Rcpp.h>
#include<vector>
#include<cmath>

#include "model.h"
#include "stats.h"
#include "model_gaussian_degenerate_zero.h"

using namespace Rcpp;

// signum of val
template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

typedef std::vector<std::string> strvec;
struct string_handler // TODO: THIS IS A COPY :(
{
  // todo: move to wrapper?
  CharacterVector concat(strvec x, strvec y)
  {
    strvec z(x.size() + y.size());
    std::copy(x.begin(), x.end(), z.begin());
    std::copy(y.begin(), y.end(), z.begin() + x.size());
    return wrap(z);
  }
};

double mycor(NumericVector x, NumericVector y)
{
  using namespace std;
  stats_functions<vector<double>> sf; 
  vector<double> xx = as<vector<double> >(x);
  vector<double> yy = as<vector<double> >(y);
  
  return sf.cor(xx, yy);
}

// [[Rcpp::export]]
NumericMatrix sgn_info_mat(NumericMatrix cont, // continuous
                           NumericMatrix dis) // discrete
{
  const int n = cont.cols(); 
  const int m = dis.cols(); 
  const int k = n + m;
  string_handler sh;
  
  NumericMatrix info_mat(k, k); // default 1 for diagonal
  
  // dim names
  const CharacterVector nms = sh.concat(as<strvec>(colnames(cont)),
                                        as<strvec>(colnames(dis)));
  rownames(info_mat) = nms;
  colnames(info_mat) = nms;
  
  // calc entries
  gaussian_degenerate_zero quad(0); // quadradic case
  gaussian_degenerate_zero linear(0); // linear case
  
  // quad terms
  for(int i = 0; i < n - 1; ++i) {
    NumericVector x1 = cont(_, i);
    info_mat(i, i) = 0; // diagonal
    
    for(int j = i + 1; j < n; ++j) {
      NumericVector x2 = cont(_, j);
      double w = quad.mutual_information(x1, x2);
      //if(w != 0) w = std::log(w);
      double rho = mycor(x1, x2); // correlation
      w = sgn(rho) * w;
      info_mat(i, j) = w;
      info_mat(j, i) = w;
    }
  }
  
  // linear terms
  for (int i = 0; i < n - 1; ++i) {
    NumericVector x1 = cont(_, i);
    
    for (int j = 0; i < m - 1; ++j) {
      NumericVector x2 = dis(_, i);
      double w = linear.mutual_information(x1, x2);
      //if(w != 0) w = std::log(w);
      double rho = mycor(x1, x2); // correlation
      w = sgn(rho) * w;
      info_mat(i, j + n) = w;
      info_mat(j + n, i) = w;
    }
  }
  
  return info_mat;
}