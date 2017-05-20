#ifndef PROGRESS_LOGGING_H
#define PROGRESS_LOGGING_H

// [[Rcpp::depends(BH)]]
#include <boost/progress.hpp>
#include <Rcpp.h>

struct output_stream : std::ostream, std::streambuf
  {
    output_stream(bool enabled) : std::ostream(this), _enabled(enabled) {}
    
    int overflow(int c)  {
      if(_enabled) Rcpp::Rcout.put(c); //std::cout.put(c);
      return 0;
    }
    
    bool _enabled;
};

class progress_bar 
{
  
  output_stream _os;
  boost::progress_display _loading_bar;
public:
  progress_bar(unsigned long int computations, bool enabled) :
  _os(enabled), 
  _loading_bar{computations, _os}
  { }
  void go() 
  {
    ++_loading_bar;
  }
};

#endif