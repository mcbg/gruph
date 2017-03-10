# gruph
Fits graphical models using kruskal's algorithm. At the moment it supports

* Vanilla Kruskal, that starts with all edges.

# Todo

* implement thresholding version of algorithm (lambda penalty)
* implement a version that takes the largest N edges of the full graph
* refracture so our kruskal function has an external policy for making our list of edges. This will make our implementation much more flexible!