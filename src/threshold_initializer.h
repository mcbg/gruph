#include "w_edge.h"
#include <vector>
#include <Rcpp.h>

using namespace Rcpp;

template<class M>
class threshold_initializer
{
  double lambda;
  M ourModel;
public:
  threshold_initializer(double l) : lambda(l), ourModel(M(l))
  {
  }
  
  std::vector<w_edge> initialize(const Rcpp::NumericMatrix xx)
  {
    std::vector<w_edge> q; // TODO: Initialize this proper
    int k = xx.ncol();
    
    for(int i = 0; i < k - 1; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(int j = i + 1; j < k; ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = ourModel.mutual_information(x, y);
        
        if (w > lambda)
          q.push_back({i, j, w, ourModel.get_df()});
      }
      
    }
    return q;
  }
};