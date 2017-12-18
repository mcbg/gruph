######################################################################
# DB TOOLS
######################################################################
 
#' @useDynLib gruph
#' @importFrom Rcpp sourceCpp
NULL

#' \code{db_insert_quad} adds all expression-expression (quadratic) edges to a database. It inserts edges
#' using chunks, to limit memory usage.
#' 
#' @param xx matrix of expression levels
#' @param tb_name string with name for DB table
#' @param chnk_size number of rows in each chunk
#' @param db connection to database 
#' 
#' @import RSQLite
#' @examples 
#' library(RSQLite)
#' mydb <- dbConnect(SQLite(), dbname = "example.db")
#' data <- as.matrix(iris[, 1:4])
#' 
#' db_insert_quad(data, "graph", 2, mydb)
#'
#' dbDisconnect(mydb)
#' @export

db_insert_quad <- function(xx, tb_name, chnk_size, db, start = 1, gaussian = FALSE) {
  
  insert_chunk_into_db <- function(k) {
    if (gaussian) {
      edges <- chunk_builder_gaussian(xx, k, chnk_size)
    } else {
      edges <- chunk_builder(xx, k, chnk_size)
    }
    
    dbWriteTable(db,
                 tb_name,
                 edges,
                 append = TRUE,
                 row.names = FALSE)
  }
  
  n <- ncol(xx)
  M <- ceiling(n / chnk_size) # num of chunks
  ch_end <-  M / 2
  k <- start
  
  while(k <= ch_end) {
    cat(k, "/", ch_end, "\n")
    insert_chunk_into_db(k)
    insert_chunk_into_db(M - k + 1)
    k <- k + 1
  }
  
  if (M %% 2 == 1) {
    insert_chunk_into_db(ceiling(M / 2))
  }
  
}

#' \code{db_insert_lin} adds all expression-clinical (linear) edges to a database. 
#' 
#' @param expr matrix of expression levels
#' @param clinical matrix of clinical variables (discrete)
#' @param tb_name string with name for DB table
#' @param db connection to database 
#' 
#' @import RSQLite
#' @examples 
#' library(RSQLite)
#' mydb <- dbConnect(SQLite(), dbname = "example.db")
#' 
#' db_insert_lin(xx, yy, "graph", mydb)
#' 
#' dbDisconnect(mydb)
#' @export

db_insert_lin <- function(expr, clinical, tb_name, db, gaussian = FALSE) {
  if (gaussian) {
    edges <- full_linear_gaussian(expr, clinical)
  } else {
    edges <- full_linear(expr, clinical)
  }
    
  dbWriteTable(db,
               tb_name,
               edges,
               append = TRUE,
               row.names = FALSE)
}

#' \code{db_max_span_forest} finds the maximum spanning forest the edges that have been stored in a database.
#' The function uses a variant of Kruskal's algorithm.Note that it saves a file called \code{forest.rds.backup}.
#' 
#' @param db connection to a database
#' @param tb_name string of table name
#' @param slices the number of slices to compute
#' 
#' @examples
#' mydb <- dbConnect(SQLite(), dbname = "example.db")
#' db_max_span_forest(mydb, 100)
#' 
#' @import RSQLite
#' @export

db_max_span_forest <- function(db, tb_name, slices) {
  
  getMinWeight <- function(x) {
    min(x$weights)
  }
  
  # ISSUE: LIMIT IS HARD CODED
  # ISSUE: table is hard coded
  mk_query <- function(minWeight) {
    paste0("SELECT *
           FROM ", tb_name, 
           " WHERE weights < ",
           minWeight,
           " ORDER BY weights DESC
           LIMIT 100000")
  }

  # ISSUE: REPLACE THIS
  minW <- 10000000
  next_edges <- dbGetQuery(db, mk_query(minW))
  minW <- getMinWeight(next_edges)
  f <- max_forest(next_edges, num_nodes = 53356, penalty = 0)

  for (slice in 1:slices) {
    if (nrow(next_edges) == 0) break
    cat(slice,": ", minW, "\n")
    next_edges <- dbGetQuery(db, mk_query(minW))
    t_edges <- rbind(f, next_edges)
    minW <- getMinWeight(t_edges)
    f <- max_forest(t_edges, num_nodes = 53356, penalty = 0)
    if (slice %% 5 == 0)
      saveRDS(f, "forest.rds.backup")
  }
  f
}