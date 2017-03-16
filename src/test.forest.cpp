#include<iostream>
#include "forest.h"

// [[Rcpp::export]]
void test_forest()
{
  std::vector<w_edge> edges;
  edges.push_back({0, {0,1}});
  edges.push_back({0, {1,2}});
  edges.push_back({0, {3,2}});
  edges.push_back({0, {0,2}});
  
  forest f(edges);
  
  std::cout << "\ninserts valid edges: ";
  f.add_edge({0, {0,1}});
  std::cout << (f.n_edges() == 1);
  f.add_edge({0, {1,2}});
  std::cout << (f.n_edges() == 2);
  f.add_edge({0, {2,3}});
  std::cout << (f.n_edges() == 3);
  
  std::cout << "\nDoesn't insert invalid edges: ";
  f.add_edge({0, {1,3}});
  std::cout << (f.n_edges() == 3);
}

/*** R
test_forest();
*/