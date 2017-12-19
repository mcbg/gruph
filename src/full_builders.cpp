
#include<vector>
#include<Rcpp.h>

#include "model_multivariate.h"
#include "model_degenerate_mixed.h"
#include "model_mixed.h"

#include "w_edge.h"
#include "wrapper.h"

using namespace std;

// [[Rcpp::export]]
List full_dinit(const Rcpp::NumericMatrix xx, int offset)
  {
    multivariate model(0);
    df_wrapper wrpr;
    ++offset;
    
    vector<w_edge> edges;
    //edges.reserve(chunk_computations(xx.ncol(), chunk_size));
    
    for (size_t i = 0; i < xx.cols() - 1 ; ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = i + 1; j < xx.cols(); ++j) {
        Rcpp::NumericVector y = xx(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        edges.push_back({i + offset, j + offset, w, df});
      }
    }
    
    return wrpr(edges);
}

// [[Rcpp::export]]
List full_linear(const Rcpp::NumericMatrix xx, // continuous
                 const Rcpp::NumericMatrix yy) // discrete
  {
    gaussian_degenerate_zero_mixed model(0);
    df_wrapper wrpr;
    int yoffset = xx.ncol() + 1;
    
    vector<w_edge> edges;
    //edges.reserve(chunk_computations(xx.ncol(), chunk_size));

    
    for (size_t i = 0; i < xx.cols(); ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = 0; j < yy.cols(); ++j) {
        Rcpp::NumericVector y = yy(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        edges.push_back({i + 1, j + yoffset, w, df});
      }
    }
    
    return wrpr(edges);
}

// [[Rcpp::export]]
List full_linear_gaussian(const Rcpp::NumericMatrix xx, 
                 const Rcpp::NumericMatrix yy)
  {
    mixed model(0);
    df_wrapper wrpr;
    int yoffset = xx.ncol() + 1;
    
    vector<w_edge> edges;
    //edges.reserve(chunk_computations(xx.ncol(), chunk_size));

    
    for (size_t i = 0; i < xx.cols(); ++i) {
      Rcpp::NumericVector x = xx(_, i);
      
      for(size_t j = 0; j < yy.cols(); ++j) {
        Rcpp::NumericVector y = yy(_, j);
        double w = model.mutual_information(x, y);
        size_t df = model.get_df();
        
        edges.push_back({i + 1, j + yoffset, w, df});
      }
    }
    
    return wrpr(edges);
}