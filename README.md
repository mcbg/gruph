# gruph
Fits graphical models using kruskal's algorithm. At the moment it supports

* calculating raw edges for gaussian, discrete, and mixed cases.
* setting a threshold

# Todo

* make secure with Principals and mechanisms  https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=637  
* get working on parellel version.
* write documentation

## assertions

* change ints to size_t
* make test for column assertion
* handle NA NaN and Inf
* test that discrete elements aren't unique!
* make tests for the above

  
# functionality

* add model for degenerate gaussian
* parallel 
* refacture threshold_init and  threshold_initializer

# testing
* compare vanilla kruskal to `grapHD`. 

# read up on 

* [Pretty code with iterators](https://stackoverflow.com/questions/11686815/custom-inserter-for-stdcopy?noredirect=1&lq=1)
* https://www.cs.helsinki.fi/u/tpkarkka/alglib/k06/

* [BOOST accumulators]( http://www.boost.org/doc/libs/1_63_0/doc/html/accumulators/user_s_guide.html)
* [BOOST for each](http://www.boost.org/doc/libs/1_63_0/doc/html/foreach.html)
* [BOOST graph](http://www.boost.org/doc/libs/1_63_0/libs/graph/doc/index.html)
