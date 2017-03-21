
#include <Rcpp.h>
#include <iostream>
#include <vector>
#include <queue>

#ifndef GAUSSIAN_MUT_INFO_H
#define GAUSSIAN_MUT_INFO_H

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

typedef std::vector<double> vecdub;
typedef std::pair<vecdub::iterator, vecdub::iterator> pairit;


class gaussian_mutual_information
{
  double mean(vecdub);
  void normalize(vecdub &);
  double sd(vecdub);
  
public:  
  double operator()(NumericVector, NumericVector);
};

#endif