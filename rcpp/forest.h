// [[Rcpp::plugins(cpp11)]] 

#include<vector>
#include<Rcpp.h>

#include "w_edge.h"
#include "set_handler.h"

/**
 * You can only insert an edge, if it does not induce a cycle.
 */

class forest
{
  std::set<int> nodes;
  std::vector<w_edge> edges;
  bool hascycle(w_edge);
  
public:
  forest(std::vector<w_edge> pEdges) : edges(pEdges) {};
  forest() {};
  void add_edge(const w_edge);
  int n_edges();
  int n_nodes();
  Rcpp::NumericMatrix get_edges();
  void print();
};