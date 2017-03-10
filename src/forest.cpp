#include "forest.h"

bool forest::hascycle(w_edge e)
{
  const int n = edges.size();
  
  std::set<int> tmpNodes(nodes);
  tmpNodes.insert(e.coord.first);
  tmpNodes.insert(e.coord.second);
  
  set_handler<int> s(tmpNodes); // save this somehow?
  
  if (edges.size() == 0) {
    return false;
  }

  for(int i = 0; i < n; ++i) {
    int x = edges[i].coord.first; 
    int y = edges[i].coord.second;
    
    if (s.get_set(x) == s.get_set(y)) {
      return true;
    } else {
      s.set_union(x, y);
    }
  }
    
  if (s.get_set(e.coord.first) == s.get_set(e.coord.second)) {
    return true;
  }
  
  return false;
}

void forest::add_edge(const w_edge e)
{
  if (!hascycle(e)) {
    edges.push_back(e);
    nodes.insert(e.coord.first);
    nodes.insert(e.coord.second);
  }
}


int forest::n_edges() {
  return edges.size();
}

int forest::n_nodes() {
  return nodes.size();
}

Rcpp::NumericMatrix forest::get_edges()
{
  int n = n_edges();
  Rcpp::NumericMatrix out(n, 2);
  
  for (int i = 0; i < n; ++i) {
    out(i, 0) = edges[i].coord.first;
    out(i, 1) = edges[i].coord.second;
  }
  
  return out;
}

void forest::print() {
  for(auto n : nodes)
    std::cout << n << ' ';
  
  std::cout << std::endl;
  
  for(auto v : edges) {
    std::cout << v.weight << ' ';
    std::cout << v.coord.first << ',' << v.coord.second << '\n';
  }
}