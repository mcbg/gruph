// [[Rcpp::plugins(cpp11)]]

#include<vector>

#include "w_edge.cpp"
#include "set_handler.cpp"

/**
 * You can only insert an edge, if it does not induce a cycle.
 */
class forest
{
  std::set<int> nodes;
  std::vector<w_edge> edges;
    
  bool hascycle(w_edge e)
  {
    const int n = edges.size();
    set_handler<int> s(nodes); // save this somehow?
  
    for(int i = 0; i < n; ++i) {
      int x = edges[i].coord.first; 
      int y = edges[i].coord.second;
      
      if (s.get_set(x) == s.get_set(y)) {
        return true;
      } else {
        s.set_union(x, y);
      }
    }
    
  if (s.get_set(e.coord.first) == s.get_set(e.coord.second))
        return true;
  
  return false;
  }
  
public:
  forest(std::vector<w_edge> pEdges) : edges(pEdges) {};
  
  void add_edge(const w_edge e)
  {
    if (!hascycle(e)) {
      mEdges.push_back(e);
      nodes.insert(e.coord.first);
      nodes.insert(e.coord.second);
    }
  }
  
};