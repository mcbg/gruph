#include "w_edge.h"
#include <vector>
#include <Rcpp.h>
#include "model.h"

using namespace Rcpp;

class threshold_initializer // TODO: new name - graph builder?
{
  const double lambda;
public:
  threshold_initializer(double l) : lambda(l)
  {
  }
  
  void add_edges_mixed(const Rcpp::NumericMatrix &xx,
             const Rcpp::NumericMatrix &yy,
             int xoffset,
             int yoffset,
             model* pModel,
             std::vector<w_edge> *edges) const
  {
    
    for(int i = 0; i < xx.ncol(); ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(int j = 0; j < yy.ncol(); ++j) {
        Rcpp::NumericVector y = yy(_, j);
        double w = pModel->mutual_information(x, y);
        
        Rcout << "W: " << w << ", lambda: " << lambda <<  std::endl;
        if (w > lambda) {
          edges->push_back({i + xoffset, j + yoffset, w, pModel->get_df()});
        }
      }
    }
  }
  
  void add_edges(const Rcpp::NumericMatrix &xx,
                  int offset,
                  model* pModel,
                  std::vector<w_edge> *edges) const
  {
    int k = xx.ncol();
    
    for(int i = 0; i < k - 1; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(int j = i + 1; j < k; ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = pModel->mutual_information(x, y);
        
        if (w > lambda)
          edges->push_back({i + offset, j + offset, w, pModel->get_df()});
      }
    }
  }
};