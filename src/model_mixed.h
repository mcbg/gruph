#include "model.h"

#include <unordered_map>
#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <queue>

#ifndef MIXED_MDL
#define MIXED_MDL

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

typedef std::vector<double> vec;

class mixed : public model
{
public:  
  mixed(double l) : model(l) { } 
  double mutual_information(variable, variable);
};

#endif
