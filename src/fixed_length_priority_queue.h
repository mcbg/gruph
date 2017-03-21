#include<queue>
#include<vector>

// [[Rcpp::plugins(cpp11)]]

template <typename T, class F>
class fl_priority_queue
{
  std::priority_queue< T, std::vector<T>, F> queue;
  F cmp = F();
  
public:
  fl_priority_queue(int n, T x)
  {
    for(int i = 0; i < n; ++i)
    {
      queue.push(x);
    }
  }
  
  bool attempt_push(T x)
  {
    if (cmp(x, queue.top()))
    {
      queue.pop();
      queue.push(x);
      return true;
    }
    return false;
  }
  
  T top()
  {
    return queue.top();
  }
  
  void copy_to_vector(T *v)
  {
    std::copy(&(queue.top()), &(queue.top()) + queue.size(), v);
  }
};
