# gruph
Fits graphical models using kruskal's algorithm. At the moment it supports

* Vanilla Kruskal, that starts with all edges.

# Todo
  * https://www.cs.helsinki.fi/u/tpkarkka/alglib/k06/
  
## functionality

* implement thresholding version of algorithm (lambda penalty)
* implement a version that takes the largest N edges of the full graph
* refracture so our kruskal function has an external policy for making our list of edges. This will make our implementation much more flexible!

## testing
* compare vanilla kruskal to `grapHD`. 
* use boost's testing framework?

## optimising

I write optimisation suggestions inline, at the moment there are two:

* vanilla_kruskal.cpp:16

## read up on 

* [BOOST accumulators]( http://www.boost.org/doc/libs/1_63_0/doc/html/accumulators/user_s_guide.html)
* [BOOST for each](http://www.boost.org/doc/libs/1_63_0/doc/html/foreach.html)
* [BOOST graph](http://www.boost.org/doc/libs/1_63_0/libs/graph/doc/index.html)
