#include "w_edge.h"

bool operator< (const w_edge lhs, const w_edge rhs)
{
  return lhs.weight > rhs.weight;   
}
