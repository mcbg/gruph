#include <Rcpp.h>


#ifndef DEGEN_MDL
#define DEGEN_MDL

using namespace Rcpp;
// [[Rcpp::plugins(cpp11)]]

typedef std::vector<double> vec;

class gaussian_degenerate_zero : public model
{
public:  
  mixed(double l) : model(l) { } 
  double mutual_information(variable, variable);
};

double gaussian_degenerate_zero::mutual_information(variable x, variable y)
{
  if (x.size() != y.size())
    stop("dimension problem")
    
  for (int i = 0; i < x.size(); ++i) {
    double p[2][2]; // probabilities
    
    if (x[i] == 0 && y[i] == 0) {
      ++p[0][0];
    } else if (x[i] == 0) {
      
    } else if (y[i] == 0) {
      
    } else {
      ++p[1][1];
      
    }
    
    for (double &pp : p) // counts to probabilities
      pp /= x.size();
  }
    
}
#endif