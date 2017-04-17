#include "model_multivariate.h"

std::set<el> multivariate::get_labels(vec x)
{
  std::set<el> setx(x.begin(), x.end());
  return setx;
}

double multivariate::mutual_information(variable px, variable py)
{
  vec x = as<vec>(px);
  vec y = as<vec>(py);
  
  std::set<el> xlabels = get_labels(x);
  std::set<el> ylabels = get_labels(y);
  
  double mutual_info = 0;
  
  // this can be optimised with maps
  for(auto xl : xlabels) {
    for(auto yl : ylabels) {
      double obs = 0;
      double xobs = 0;
      double yobs = 0;
      
      for (int i = 0; i < x.size(); ++i) {
        if (y[i] == yl && x[i] == xl)
            ++obs;
        if (y[i] == yl)
            ++yobs;
        if (x[i] == xl)
          ++xobs;
      }
      
      double expected = (xobs * yobs) / x.size();
      std::cout << obs << std::endl;
      mutual_info += std::pow(obs - expected, 2) / expected;
    }
  }
  
  // SET DF
  df = (xlabels.size() - 1) * (ylabels.size() - 1);
  
  // lower tail: false, log: false
  return mutual_info / (2 * x.size());
}
