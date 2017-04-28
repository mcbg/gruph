// [[Rcpp::plugins(cpp11)]]

#include<unordered_map>
#include<set>
#include<iostream>

template<typename T>
class set_handler
{
  std::unordered_map<T, int> sets;
  int group_size;
  
public:
  set_handler(std::set<T> vs) :
    group_size(0)
  // add each vertex in v
  {
    for(auto v : vs) {
      sets[v] = group_size++;
    }
  }
  
  void insert_vertex(T e) {
    // put vertex in new set, if it is new
    auto it = sets.find(e);
    if (it == sets.end())
      sets[e] = group_size++;
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
