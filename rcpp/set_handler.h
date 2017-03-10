// [[Rcpp::plugins(cpp11)]]

#include<unordered_map>
#include<set>
#include<iostream>

template<typename T>
class set_handler
{
  std::unordered_map<T, int> sets;
  
public:
  set_handler(std::set<T> v)
  {
    int i = 0;
    
    for(auto e : v) {
      sets[e] = i++;
    }
  }
  
  void set_union(T x, T y)
  {
    int xSet = sets[x];
    int ySet = sets[y];
    
    for(auto &pair : sets) {
      if (pair.second == ySet) {
        pair.second = xSet;
      }
    }
  }
  
  void print()
  {
    for(auto pair : sets) {
      std::cout << pair.first << " : " << pair.second << "\n\n";
    }
  }
  
  int get_set(T x)
  {
    return sets[x];
  }
};
