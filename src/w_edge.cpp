#include "w_edge.h"
bool operator< (const w_edge lhs, const w_edge rhs)
{
  return std::abs(lhs.weight) < std::abs(rhs.weight);   
}
