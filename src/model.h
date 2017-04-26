#include<Rcpp.h>
#include "w_edge.h"

#ifndef MODEL_H
#define MODEL_H

typedef Rcpp::NumericVector variable;

/**
 * `model` is an interface for interacting with different models' 
 * mutual information.
 */

class model
{
  double lambda; // penalty
protected:
  int df; // degrees of freedom
public:
  model(void);
  model(double);
  // returns MI, and sets `df`.
  virtual double mutual_information(variable, variable) = 0;
  double aic(variable, variable); // an information criterium
  double penalized_information(variable, variable);
  int get_df() const;
};

#endif