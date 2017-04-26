// [[Rcpp::plugins(cpp11)]]

#include "model_mixed.h"

double mixed::mutual_information(variable x, variable y)
{
  Rcout << "sup";
  testing<variable> t;
  return t.anova_chisq(x, y, &df);
}