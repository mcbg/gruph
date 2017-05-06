#include<utility>
#include<cmath>

#ifndef EDGE_H
#define EDGE_H

struct w_edge {
  size_t i;
  size_t j;
  double weight;
  size_t df;
  
};

struct w_edge_greater 
{
  bool operator() (const w_edge lhs, const w_edge rhs)
  {
    return lhs.weight >  rhs.weight;
  }
};

bool operator< (const w_edge lhs, const w_edge rhs);

#endif