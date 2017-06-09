// [[Rcpp::plugins(cpp11)]]

#include "fl_heap.h"
#include "w_edge.h"
#include "wrapper.h"
#include "model_gaussian_degenerate_zero.h"

#include<Rcpp.h>

using namespace Rcpp;

constexpr double penalized_ML(w_edge x, double lambda)
{
  return x.weight - lambda * x.df;
}

template<class T, class M>
void add_edges(T &edges, NumericMatrix xx, int offset)
{
  M model(0);
  for (size_t i = 0; i < xx.cols(); ++i) {
    Rcpp::NumericVector x = xx(_, i);
    
    for (size_t j = i + 1; j < xx.cols(); ++j) {
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
                     int offset2)
{
  M model(0);
  for (size_t i = 0; i < xx.cols(); ++i) {
    Rcpp::NumericVector x = xx(_, i);
    
    for(size_t j = 0; j < yy.cols(); ++j) {
      Rcpp::NumericVector y = yy(_, j);
      double w = model.mutual_information(x, y);
      size_t df = model.get_df();
      w_edge e{i + offset, j + offset2, w, df};
      
      edges.insert(e);
    } 
  }
}

struct compare_edges
{
  compare_edges(double ll) : lambda(ll) {} 
  compare_edges() : lambda(0) {} 
  
  bool operator()(w_edge x, w_edge y)
  {
    return penalized_ML(x, lambda) > penalized_ML(y, lambda);
  }
private:
  const double lambda;
};


/** find homogenous edges---between variables of the same distribution. Has
 * two modes "degenerate" and discrete.
 */
// [[Rcpp::export]]
List fixed_graph_builder(Rcpp::NumericMatrix xx,
                                    int queue_size,
                                    double penalty)
{
  typedef fl_heap<w_edge, compare_edges> myqueue;
  myqueue edges(queue_size, penalty);
  df_wrapper wrpr;
  
  add_edges<myqueue, gaussian_degenerate_zero>(edges, xx, 1);
  
  auto v = edges.finalize();
  return wrpr(v);
}