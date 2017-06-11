#ifndef DEGEN_MIX_MDL
#define DEGEN_MIX_MDL

// [[Rcpp::plugins(cpp11)]]

#include "model.h"
#include "model_multivariate.h"
#include <unordered_map>

class gaussian_degenerate_zero_mixed : public model
{
  /**
   * This uses Steffens approximation
   */
public:
  multivariate mult;
  gaussian_degenerate_zero_mixed(double l) : model(l), mult(l) {} 
  
  /**
   * param u: continous variable with degenerate zero
   * param d: discrete variable
   * 
   * changes df
   * returns: mutual information between variables
   */
  double mutual_information(variable, variable);
};

#endif
