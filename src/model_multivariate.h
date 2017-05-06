// [[Rcpp::plugins(cpp11)]]
#ifndef MULT_MDL
#define MULT_MDL

#include "model.h"
#include <Rcpp.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace Rcpp;

typedef double el;
typedef std::vector<el> vec;

class multivariate : public model
{
  std::set<el> get_labels(vec) const;
  
public:  
  multivariate(double l) : model(l) {}
  double mutual_information(variable, variable);
};

#endif