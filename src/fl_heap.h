#include<queue>
#include<Rcpp.h>
#include<vector>
#include<algorithm>

// [[Rcpp::plugins(cpp11)]]

/**
 * T: type stored in underlying vector
 * F: struct containing functions to compare elements
 * 
 * based on: https://stackoverflow.com/questions/33180609/how-to-create-a-min-heap-of-fixed-size-using-stdpriority-queue
 */

using namespace std;
template <typename T, class F>

class fl_heap {
public:
  fl_heap(int k, double l) : k_(k), cmp_(l) {}
  
  void insert(T value)
  {
    // if we are under our limit, we just add the element
    if (c_.size() < k_) {
      c_.push_back(value);
      if (c_.size() == k_) make_heap(c_.begin(), c_.end());
    }
    else if (cmp_(value, c_[0])) {
      /* See note below */
      pop_heap(c_.begin(), c_.end(), cmp_); 
      c_.back() = value;
      push_heap(c_.begin(), c_.end(), cmp_);
    }
  }
  std::vector<T> finalize()
  {
    // returns vector sorted from largest to smallest
    // DONT USE `fl_heap` OBJECT AFTER FINALISING!
    
    if (c_.size() < k_)
      std::sort(c_.begin(), c_.end(), cmp_);
    else
      sort_heap(c_.begin(), c_.end(), cmp_);
    
    std::vector<T> c;
    std::swap(c, c_); // this makes `c_` unusable
    
    return c; // Makes fl_heap object unusable!
  }
  
private:
  /* invariant: if c_.size() == k, then c_ is a maxheap. */
  size_t k_;
  std::vector<T> c_;
  F cmp_;
};
