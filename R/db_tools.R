######################################################################
# DB TOOLS
######################################################################
 
#' @useDynLib gruph
#' @importFrom Rcpp sourceCpp
#' @importFrom graphics par plot
#' @importFrom stats df pchisq rnorm
NULL

#' \code{db_insert_quad} adds all expression-expression (quadratic) edges to a database. It inserts edges
#' using chunks, to limit memory usage.
#' 
#' @param xx matrix of expression levels
#' @param tb_name string with name for DB table
#' @param chnk_size number of rows in each chunk
#' @param db connection to database 
#' @param start which chunk to start at 
#' @param gaussian uses gaussian distribution if true; degenerate if false
#' 
#' @import RSQLite
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
#' @param gaussian uses gaussian distribution if true; degenerate if false
#' 
#' @import RSQLite
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

