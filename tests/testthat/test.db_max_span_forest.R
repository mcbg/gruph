library(igraph, quietly = TRUE, verbose = FALSE)

context('db_max_span_forest')

set.seed(123)
g <- make_full_graph(20,directed = FALSE)
our_edgelist <- data.frame(as_edgelist(g))
names(our_edgelist) <- c('i', 'j')
w <- rnorm(nrow(our_edgelist))
our_edgelist$weights <- w[order(w, decreasing = TRUE)]
our_edgelist$df <- 1

test_that('result is a tree', {
  
  with_mock(next_query = function(db, tb_name, chunk_size, minW = 0, init = FALSE) {
    if (init) {
      our_edgelist[1:chunk_size, ]
   } else {
      our_edgelist[our_edgelist$weights < minW,][1:chunk_size, ]
    }
  } , saveRDS = function(...) {}, { # NO BACKUP FILE!
   x <- db_max_span_forest(db=NULL, 'hi', 20, slices = 18, chunk_size = 10, verbose = FALSE)
   expect_true(nrow(x) == 19) # less edges than vertices
  })
})
