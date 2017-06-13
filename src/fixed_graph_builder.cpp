// [[Rcpp::plugins(cpp11)]]

#include "fl_heap.h"
#include "w_edge.h"
#include "wrapper.h"
#include "output.h"

#include "model_gaussian_degenerate_zero.h"
#include "model_multivariate.h"
#include "model_degenerate_mixed.h"

#include<Rcpp.h>

using namespace Rcpp;

constexpr double penalized_ML(w_edge x, double lambda)
{
  return x.weight - lambda * x.df;
}

template<class T, class M>
void add_edges(T &edges, NumericMatrix xx, int offset, bool verbose)
{
  // stuff for progress bar
  const size_t computations = xx.ncol() * (xx.ncol() - 1) * 0.5;
  progress_bar progress(computations, verbose);
  
  M model(0);
  for (size_t i = 0; i < xx.cols() - 1; ++i) {
    Rcpp::NumericVector x = xx(_, i);
    
    for (size_t j = i + 1; j < xx.cols(); ++j) {
      progress.go();
      Rcpp::NumericVector y = xx(_, j);
      double w = model.mutual_information(x, y);
      size_t df = model.get_df();
      
      w_edge e{i + offset, j + offset, w, df};
      edges.insert(e);
    } 
  }
}

template<class T, class M>
void add_edges_mixed(T &edges,
                     NumericMatrix xx,
                     NumericMatrix yy,
                     int offset,
                     int offset2,
                     bool verbose)
{
  // stuff for progress bar
  const size_t computations = xx.ncol() * yy.ncol();
  progress_bar progress(computations, verbose);

  M model(0);
  for (size_t i = 0; i < xx.cols(); ++i) {
    Rcpp::NumericVector x = xx(_, i);
    
    for(size_t j = 0; j < yy.cols(); ++j) {
      Rcpp::NumericVector y = yy(_, j);
      double w = model.mutual_information(x, y);
      size_t df = model.get_df();
      w_edge e{i + offset, j + offset2, w, df};
      
      edges.insert(e);
      progress.go();
    } 
  }
}

struct compare_edges
{
  compare_edges(double ll) : lambda(ll) {} 
  compare_edges() : lambda(0) {} 
  
  bool operator()(w_edge x, w_edge y)
  {
    if (x.df == 0 && y.df != 0)
      return false;
    if (y.df == 0 && x.df != 0)
      return true;
    
    return penalized_ML(x, lambda) > penalized_ML(y, lambda);
  }
private:
  const double lambda;
};


/** find homogenous edges---between variables of the same distribution. Has
 * two modes "degenerate" and discrete.
 * 
 * Finds the top `queue_size` edges wrt penalized ML.
 */
// [[Rcpp::export]]
DataFrame fl_dginit(Rcpp::NumericMatrix xx, // degenerate
                         Rcpp::NumericMatrix yy, // discrete
                         int queue_size,
                         double penalty,
                         bool verbose)
{
  if (xx.nrow() != yy.nrow())
    Rcpp::stop("different number of rows in xx and yy");
  
  typedef fl_heap<w_edge, compare_edges> myqueue;
  
  // edges is the only component that uses our penalty
  myqueue edges(queue_size, penalty); 
  df_wrapper wrpr;
  
  const size_t disrete_offset = xx.ncol() + 1;
  add_edges<myqueue, gaussian_degenerate_zero>(edges, xx, 1, verbose);
  add_edges<myqueue, multivariate>(edges, yy, disrete_offset, verbose);
  add_edges_mixed<myqueue, gaussian_degenerate_zero_mixed>(edges,
                                                           xx,
                                                           yy,
                                                           1,
                                                           disrete_offset,
                                                           verbose);
  
  auto v = edges.finalize();
  return wrpr(v);
}
