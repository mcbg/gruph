// REFRACTOR!!
#include <Rcpp.h>
#include "forest.h"
#include "gaussian_mutual_information.h"
#include "fixed_length_priority_queue.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

/**
 * Creates a forest with a maximum of N edges.
 */

// [[Rcpp::export]]
NumericMatrix limited_kruskal(NumericMatrix m, int lim)
{
  fl_priority_queue<w_edge, w_edge_greater> q(lim, {0, {0, 0}});
  std::vector<w_edge> edges(lim, {0, {0, 0}});
  
  for (int i = 0; i < m.ncol();  ++i)
  {
    NumericVector x = m(_, i);
    
    for (int j = i + 1; j < m.ncol();  ++j)
    {
      NumericVector y = m(_, j);
      double w = gaussian_mutual_information()(x, y);
      q.attempt_push({ w , {i, j}});
    }
  }
  
  q.copy_to_vector(&edges[0]);
  std::sort(edges.begin(), edges.end(), w_edge_greater());
  
  forest f(edges);
  
  return f.get_edges();
}

