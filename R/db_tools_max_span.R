#' @import RSQLite
next_query <- function(db, tb_name, chunk_size, minW = 0, init = FALSE) {
  mk_query <- function(minWeight) {
    paste0("SELECT *
           FROM ", tb_name, 
           " WHERE weights < ",
           minWeight,
           " ORDER BY weights DESC
           LIMIT ", chunk_size)
  }
  
  #ISSUE put in try-catch 
  if (init) {
    dbGetQuery(db, paste0('SELECT * from ', tb_name, ' ORDER BY weights DESC limit ', chunk_size))
  } else {
    dbGetQuery(db, mk_query(minW))
  }
}

#' \code{db_max_span_forest} finds the maximum spanning forest the edges that have been stored in a database.
#' The function uses a variant of Kruskal's algorithm.Note that it saves a file called \code{forest.rds.backup}.
#' 
#' @param db connection to a database
#' @param tb_name string of table name
#' @param n the number of vertices in database
#' @param slices the number of slices to compute
#' @param chunk_size the size of chunks that will be taken from the database
#' @param verbose send output w.r.t. how the process is running
#' 
#' @export
db_max_span_forest <- function(db, tb_name, n, slices, chunk_size=100000L, verbose = TRUE) {
  
  # preamble
  getMinWeight <- function(x) {
    min(x$weights)
  }

  chunk_size <- format(chunk_size, scientific = FALSE) # make sure scientific notation doesn't creep in
  
  # initiate
  next_edges <- next_query(db, tb_name, chunk_size, init=TRUE)
  minW <- getMinWeight(next_edges)
  f <- max_forest(next_edges, num_nodes = n, penalty = 0)

  # main loop
  for (slice in 1:slices) {
    # break if no more edges
    if (nrow(next_edges) == 0) break
    
    # next iteration
    if (verbose) cat(slice,": ", minW, "\n")
    
    next_edges <- next_query(db, tb_name, chunk_size, minW, init=FALSE)
    t_edges <- rbind(f, next_edges)
    minW <- getMinWeight(t_edges)
    f <- max_forest(t_edges, num_nodes = n, penalty = 0)
    
    # save back up
    if (slice %% 5 == 0)
      saveRDS(f, "forest.rds.backup")
  }
  f
}