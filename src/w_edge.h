#include<utility>
#include<cmath>

#ifndef EDGE_H
#define EDGE_H

struct w_edge {
  double weight;
  std::pair<int, int> coord;
};

bool operator< (const w_edge lhs, const w_edge rhs);

#endif