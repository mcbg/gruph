#include<Rcpp.h>
#include<cmath>

#include "fixed_length_priority_queue.cpp"
#include "gaussian_mutual_information.cpp"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

typedef std::pair<int, int> edge;
typedef std::pair<double, edge> w_edge;

template<class mutual_info>
std::vector<w_edge> get_large_edges (NumericMatrix xx, size_t queue_size)
{
  class w_edge_greater
  {
  public:
    bool operator()(w_edge lhs, w_edge rhs)
    {
      return std::abs(lhs.first) > std::abs(rhs.first); 
    }
  };
  
  mutual_info mutual_information = mutual_info();
  
  fl_priority_queue<w_edge, w_edge_greater> q(queue_size, {0, {0, 0}}); 
  std::vector<w_edge> outvec(queue_size, {0, {0, 0}});
  
  for (int i = 0; i < xx.ncol();  ++i)
  {
    NumericVector x = xx(_, i);
    
    for (int j = i + 1; j < xx.ncol();  ++j)
    {
      NumericVector y = xx(_, j);
      double w = mutual_information(x, y);
      q.attempt_push({ w , {i, j}});
    }
  }
  
  q.copy_to_vector(&outvec[0]);
  return(outvec);
  
}

// [[Rcpp::export]]
void printedges(NumericMatrix m) {
  std::vector<w_edge> v =  get_large_edges<gaussian_mutual_information>(m, 8);
  for(auto x : v) std::cout << x.second.first << "," << x.second.second << "\n"; 
}

/*** R
printedges(matrix(rnorm(100), nrow = 20))
*/