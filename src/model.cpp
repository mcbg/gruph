#include "model.h"

model::model() : lambda(0)
{
    
}

model::model(double l) : lambda(l)
{
    
}

double model::aic(variable x, variable y)
{
  double info = mutual_information(x, y);
  info -= df;
  return info;
}

double model::penalized_information(variable x, variable y)
{
  double info = mutual_information(x, y);
  info -= lambda * df;
  return info;
}

int model::get_df()
{
  return df;
}