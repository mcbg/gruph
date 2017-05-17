// [[Rcpp::depends(BH)]]
#include<Rcpp.h>
using namespace Rcpp;


/** Wraps edges into nice R object.
*/
struct wrapper 
{
  template<typename T>
  void operator() (T edges,
                  const CharacterVector &colnames,
                  NumericMatrix *out)
  {
    /** 
     * Populates `out` with the contents of `edges`.
     */
    
    // sort
    std::sort(begin(edges), end(edges));
    int n = edges.size();
    NumericVector weights(n);
    NumericVector df(n);
    
    auto node_style = [edges, colnames] (int i, bool is_first) {
      int k = is_first ? edges[i].i : edges[i].j;
      //return colnames[k];
      return k++;
    };
    
    for (int i = 0; i < n; ++i) {
      weights[i] = edges[i].weight;
      df[i] = edges[i].df;
      
      
      (*out)(i, 0) = node_style(i, true);
      (*out)(i, 1) = node_style(i, false);
      
    }
    
    out->attr("weights") = weights;
    out->attr("df") = df;
    out->attr("columns") = colnames;
  }

};
