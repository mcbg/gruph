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
  // members
  std::set<int> nodes;
  std::vector<w_edge> edges;
  set_handler<int> mSetHandler;
  
  // private functions
  bool has_cycle(w_edge);
  
public:
  forest(std::vector<w_edge>);
  
  // modifier 
  void add_edge(const w_edge);
  
  // observers
  int n_edges();
  int n_nodes();
  Rcpp::NumericMatrix get_edges();
  void print();
};