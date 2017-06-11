// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins(cpp11)]]

#include <Rcpp.h>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include "w_edge.h"
#include "wrapper.h"

using namespace Rcpp;
using namespace std;
using namespace boost;

// inverts the `<` operator, so we find max forest
struct inv_double
{
  inv_double(double pw,
             int pdf,
             double penalty)
  : w(pw),
    df(pdf),
    x(pw - pdf * penalty) {}
  
  inv_double() : inv_double(0, 0, 0) {} 
  
  double w;
  int df;
  double x;
};

bool operator>(inv_double x, inv_double y) {
  return x.x < y.x;
}

bool operator<(inv_double x, inv_double y)
{
  return x.x > y.x;
}

bool operator<=(inv_double x, inv_double y)
{
  return !(x > y);
}

bool operator>=(inv_double x, inv_double y)
{
  return !(x < y);
}

// [[Rcpp::export]]
DataFrame max_forest(DataFrame x, int num_nodes, double penalty)
{
  typedef vector<inv_double> Weights; 
  typedef pair<int, int> E; 
  typedef vector<E> Edges; 
  
  typedef property<edge_weight_t, inv_double> EdgeWeightProperty;
  typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> graph;
  typedef graph_traits < graph >::edge_descriptor Edge;
  
  NumericVector first = x["i"];
  NumericVector second = x["j"];
  NumericVector w = x["weights"];
  NumericVector df = x["df"];
  
  Edges edges_(x.nrow(), {-1, -1});
  Weights weights_(w.size());
  
  for(int i = 0; i < x.nrow(); ++i) {
    edges_[i].first = first[i];
    edges_[i].second = second[i];
    
    weights_[i] = inv_double(w[i], df[i], penalty);
  }
  
  // MAKE BOOST GRAPH
  graph g(edges_.begin(), edges_.end(), weights_.begin(), num_nodes);
 
  std::vector< Edge > spanning_forest;
  kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_forest));
  
  // export
  const size_t k = spanning_forest.size();
  NumericVector out_first(k);
  NumericVector out_second(k);
  NumericVector out_w(k);
  NumericVector out_df(k);
  
  int i = 0;
  for (std::vector < Edge >::iterator ei = spanning_forest.begin();
       ei != spanning_forest.end();
       ++ei, ++i) {
    out_first(i) = source(*ei, g);
    out_second(i) = target(*ei, g);
    out_w(i) = get(edge_weight, g, *ei).w;
    out_df(i) = get(edge_weight, g, *ei).df;
  }
  
  return DataFrame::create(_("i") = out_first,
                   _("j") = out_second,
                   _("weights") = out_w,
                   _("df") = out_df);
}

