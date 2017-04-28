# gruph
Fits graphical models using kruskal's algorithm. At the moment it supports

* calculating raw edges for gaussian, discrete, and mixed cases.
* setting a threshold

# Todo

* get forest stuff to work with new `w_edge`.
* get working on parellel version.

* write documentation
  
## functionality

* make return value have `R` entries (just plus one)
* refracture so our kruskal function has an external policy for making our list of edges. This will make our implementation much more flexible!

## testing
* compare vanilla kruskal to `grapHD`. 

## read up on 

* https://www.cs.helsinki.fi/u/tpkarkka/alglib/k06/

* [BOOST accumulators]( http://www.boost.org/doc/libs/1_63_0/doc/html/accumulators/user_s_guide.html)
* [BOOST for each](http://www.boost.org/doc/libs/1_63_0/doc/html/foreach.html)
* [BOOST graph](http://www.boost.org/doc/libs/1_63_0/libs/graph/doc/index.html)
