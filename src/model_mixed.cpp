// [[Rcpp::plugins(cpp11)]]

#include "model_mixed.h"
#include "stats.h"

double mixed::mutual_information(variable x, variable y)
{
  testing<variable> t;
  return t.anova_chisq(x, y, &df);
}
