#include "forest.h"

std::set<int> get_nodes(const std::vector<w_edge> edges)
{
    std::set<int> tmpNode;
  
    for(auto edge : edges) {
      tmpNode.insert(edge.i);
      tmpNode.insert(edge.j);
    }
    
    return tmpNode;
}

std::vector<w_edge> hi (Rcpp::NumericMatrix edges, Rcpp::NumericVector weight) {
  std::vector<w_edge> out(edges.nrow());
  for(int i = 0; i < edges.nrow(); ++i) {
    out[i].weight = weight(i);
    out[i].i = edges(i, 0);
    out[i].j = edges(i, 1);
  }
  return out;
}

forest::forest(const Rcpp::NumericMatrix edges, const Rcpp::NumericVector weights) : 
  forest(hi(edges, weights))
{
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
  return mSetHandler.get_set(e.i) == mSetHandler.get_set(e.j);
}

void forest::add_edge(const w_edge e)
{
  if (!has_cycle(e)) {
    edges.push_back(e);
    mSetHandler.set_union(e.i, e.j);
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
  Rcpp::NumericVector w(n);
  
  for (int i = 0; i < n; ++i) {
    out(i, 0) = edges[i].i;
    out(i, 1) = edges[i].j;
    w(i) = edges[i].weight;
  }
  out.attr("weights") = w;
  return out;
}

void forest::print() {
  for(auto n : nodes)
    std::cout << n << ' ';
  
  std::cout << std::endl;
  
  for(auto v : edges) {
    std::cout << v.weight << ' ';
    std::cout << v.i << ',' << v.j << '\n';
  }
}