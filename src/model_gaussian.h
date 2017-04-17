#include "model.h"

#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <queue>

#ifndef GAUSSIAN_MUT_INFO_H
#define GAUSSIAN_MUT_INFO_H

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

typedef std::vector<double> vec;

class gaussian : public model
{
public:  
  gaussian(double l) : model(l) {
    df = 1;
  } 
  double mutual_information(variable, variable);
};

#endif