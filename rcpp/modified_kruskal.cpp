#include<Rcpp.h>
#include<cmath>

#include "fixed_length_priority_queue.cpp"
#include "gaussian_mutual_information.cpp"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

typedef std::pair<int, int> edge;
typedef std::pair<double, edge> w_edge;

template<class mutual_info>
void get_large_edges (NumericMatrix xx)
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
  
  const int queue_size = 7;
  fl_priority_queue<w_edge, w_edge_greater> q(queue_size, {0, {0, 0}}); 
  std::vector<w_edge> outvec(queue_size, {0, {0, 0}});
  
  for (int i = 0; i < xx.ncol();  ++i) {
    NumericVector x = xx(_, i);
    for (int j = i + 1; j < xx.ncol();  ++j) {
      NumericVector y = xx(_, j);
      double w = mutual_information(x, y);
      q.attempt_push({ w , {i, j}});
    }
  }
  
  q.copy_to_vector(&outvec[0]);
  
  auto print_element = [] (w_edge e)
  {
    std::cout << '(' << e.second.first << ", " << e.second.second << ") ";
    std::cout << e.first << '\n';
  };
  
  for(auto x : outvec) print_element(x);
  
}

// [[Rcpp::export]]
void hi (NumericMatrix xx) {
  get_large_edges<gaussian_mutual_information>(xx);
}