#include "w_edge.h"
#include "model.h"
#include "output.h"

#include <vector>
#include <Rcpp.h>


using namespace Rcpp;

class graph_builder 
{
  const double lambda;
  const bool output_enabled;
public:
  graph_builder(double l, bool s) : lambda(l), output_enabled(s) { }
  graph_builder(double l) : lambda(l), output_enabled(false) { }
  
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

    progress_bar progress(computations, output_enabled);
    
    for(size_t i = 0; i < xcols; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = 0; j < ycols; ++j) {
        Rcpp::NumericVector y = yy(_, j);
        
        double w = pModel->mutual_information(x, y);
        size_t df = pModel->get_df();
        
        if (w > lambda * df) {
          edges->push_back({i + xoffset, j + yoffset, w, df});
        } else {
          //Rcpp::warning("lol");
        }
        
        progress.go();
      }
    }
  }
  
  void add_edges(const Rcpp::NumericMatrix &xx,
                  int offset,
                  model* pModel,
                  std::vector<w_edge> *edges) const
  {
    const size_t k = xx.ncol();
    const size_t computations = k * (k-1) / 2;
    
    progress_bar progress(computations, output_enabled);
    
    for(size_t i = 0; i < k - 1; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = i + 1; j < k; ++j) {
        progress.go();
        
        Rcpp::NumericVector y = xx(_, j);
        double w = pModel->mutual_information(x, y);
        if (w > lambda)
          edges->push_back({i + offset, j + offset, w, pModel->get_df()});
      }
    }
  }
};
