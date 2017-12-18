#include<algorithm>
#include<Rcpp.h>

#include "model_gaussian_degenerate_zero.h"
#include "model_gaussian.h"
#include "w_edge.h"
#include "wrapper.h"

using namespace Rcpp;

inline size_t chunk_computations(int n, int k)
{
  return n * k - k * (k - 1) * 0.5;
}


// [[Rcpp::export]]
List chunk_builder(const Rcpp::NumericMatrix xx, int chunk_num, int chunk_size)
  {
    if (chunk_size > xx.ncol()) { // Make sure chunks are smaller than the number of columns
      chunk_size = xx.ncol();
      warning("chunk_size too large, using the number of columns instead.");
    }
    
    gaussian_degenerate_zero model(0);
    df_wrapper wrpr;
    
    vector<w_edge> edges;
    edges.reserve(chunk_computations(xx.ncol(), chunk_size));

    --chunk_num;
    int start = chunk_num * chunk_size;
    int r_end = std::min(start + chunk_size, xx.cols());
    
    for (size_t i = start; i < r_end; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for (size_t j = i + 1; j < xx.cols(); ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        edges.push_back({i + 1, j + 1, w, df});
      }
    }
    
    return wrpr(edges);
}


// [[Rcpp::export]]
List chunk_builder_gaussian(const Rcpp::NumericMatrix xx, int chunk_num, int chunk_size)
  {
    if (chunk_size > xx.ncol()) { // Make sure chunks are smaller than the number of columns
      chunk_size = xx.ncol();
      warning("chunk_size too large, using the number of columns instead.");
    }
    
    gaussian model(0);
    df_wrapper wrpr;
    
    vector<w_edge> edges;
    edges.reserve(chunk_computations(xx.ncol(), chunk_size));

    --chunk_num;
    int start = chunk_num * chunk_size;
    int r_end = std::min(start + chunk_size, xx.cols());
    
    for (size_t i = start; i < r_end; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for (size_t j = i + 1; j < xx.cols(); ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        edges.push_back({i + 1, j + 1, w, df});
      }
    }
    
    return wrpr(edges);
}