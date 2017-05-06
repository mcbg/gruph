#include "w_edge.h"
#include "model.h"

#include <vector>
#include <Rcpp.h>
#include <boost/progress.hpp>

// [[Rcpp::depends(BH)]]

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
    const unsigned int xcols = xx.ncol();
    const unsigned int ycols = yy.ncol();
    const size_t computations = size_t(xx.ncol()) * size_t(yy.ncol());
    boost::progress_display loading_bar(computations, Rcout);
    
    for(size_t i = 0; i < xcols; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = 0; j < ycols; ++j) {
        Rcpp::NumericVector y = yy(_, j);
        double w = pModel->mutual_information(x, y);
        size_t df = pModel->get_df();
        
        if (w > lambda * df) {
          edges->push_back({i + xoffset, j + yoffset, w, df});
        }
        
        ++loading_bar;
      }
    }
  }
  
  void add_edges(const Rcpp::NumericMatrix &xx,
                  int offset,
                  model* pModel,
                  std::vector<w_edge> *edges) const
  {
    const size_t k = xx.ncol();
    const size_t computations = k * k / 2;
    
    boost::progress_display loading_bar(computations, Rcout);
    
    for(size_t i = 0; i < k - 1; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = i + 1; j < k; ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = pModel->mutual_information(x, y);
        
        if (w > lambda)
          edges->push_back({i + offset, j + offset, w, pModel->get_df()});
        ++loading_bar;
      }
    }
  }
};