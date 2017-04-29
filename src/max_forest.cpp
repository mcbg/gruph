// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins(cpp11)]]

#include <Rcpp.h>
//#include "forest.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>


using namespace Rcpp;
using namespace boost;

struct inv_double
{
    //just inverts `operator<` so to find maximum forest
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
NumericVector max_forest(NumericMatrix x) {
  
  typedef std::vector<inv_double> Weights;
  typedef std::pair<int, int> E;
  typedef std::vector<E> edges;
  
  size_t num_nodes = x.ncol();
  NumericVector w = x.attr("weights");
  
  // CONVERT TO CPP OBJECTS
  edges mEdges(x.nrow(), {-1, -1});
  Weights weights(w.size()); // = as<Weights>(w);
  
  for(int i = 0; i < x.nrow(); ++i) {
    mEdges[i].first = x(i,0);
    mEdges[i].second = x(i,1);
    weights[i].x = w(i);
  }
  
  
  // make boost graph
  typedef property<edge_weight_t, inv_double> EdgeWeightProperty;
  typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> graph;
  
  graph g(mEdges.begin(), mEdges.end(), weights.begin(), num_nodes);
 
  typedef graph_traits < graph >::edge_descriptor Edge;
  std::vector< Edge > spanning_forest;
  kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_forest));
  
  // export
  NumericMatrix out(spanning_forest.size(), 2);
  NumericVector out_w(spanning_forest.size());
  
  int i = 0;
  for (std::vector < Edge >::iterator ei = spanning_forest.begin();
       ei != spanning_forest.end();
       ++ei, ++i) {
    out(i, 0) = source(*ei, g);
    out(i, 1) = target(*ei, g);
    out_w(i) = get(edge_weight, g, *ei).x;
  }
  out.attr("weights") = out_w;
  return out;
}
