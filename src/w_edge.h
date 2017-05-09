#include<utility>
#include<cmath>
#include<Rcpp.h>

#ifndef EDGE_H
#define EDGE_H

typedef int64_t our_int;

struct w_edge {
  our_int i;
  our_int j;
  double weight;
  our_int df;
  
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