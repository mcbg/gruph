#include "forest.h"

std::set<int> get_nodes(const std::vector<w_edge> edges)
{
    std::set<int> tmpNode;
  
    for(auto edge : edges) {
      tmpNode.insert(edge.coord.first);
      tmpNode.insert(edge.coord.second);
    }
    
    return tmpNode;
}

forest::forest(const std::vector<w_edge> edges)
  :
  // initiates nodes based on edges
  nodes(get_nodes(edges)),
  // each node starts in its own set
  mSetHandler(nodes)
{
  // insert each edge that doesn't induce cycle
  for(auto e : edges)
    add_edge(e);
}

bool forest::has_cycle(w_edge e)
{
  return mSetHandler.get_set(e.coord.first) == mSetHandler.get_set(e.coord.second);
}

void forest::add_edge(const w_edge e)
{
  if (!has_cycle(e)) {
    edges.push_back(e);
    mSetHandler.set_union(e.coord.first, e.coord.second);
  }
}

int forest::n_edges()
{
  return edges.size();
}

int forest::n_nodes()
{
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