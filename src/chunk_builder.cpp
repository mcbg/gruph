#include<algorithm>
#include<Rcpp.h>

#include "model_gaussian_degenerate_zero.h"
#include "w_edge.h"
#include "wrapper.h"

using namespace Rcpp;


// [[Rcpp::export]]
List chunk_add_edges(const Rcpp::NumericMatrix xx,  double lambda, int start, int chuck_size)
  {
    gaussian_degenerate_zero model(lambda);
    df_wrapper wrpr;
    
    vector<w_edge> edges;
    size_t r_end = std::min(start + chuck_size - 1, xx.cols());
    
    for (size_t i = start - 1; i < r_end; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = i + 1; j < xx.cols(); ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        if (w > lambda * df) {
          edges.push_back({i + 1, j + 1, w, df});
        } 
      }
    }
    
    return wrpr(edges);
}

