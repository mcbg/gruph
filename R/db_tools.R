######################################################################
# DB TOOLS
######################################################################

#' \code{db_insert_quad} adds all expression-expression (quadratic) edges to a database. It inserts edges
#' using chunks, to limit memory usage.
#' 
#' @param xx matrix of expression levels
#' @param tb_name string with name for DB table
#' @param chnk_size number of rows in each chunk
#' @param db connection to database 
#' 
#' @examples 
#' libary(RSQLite)
#' mydb <- dbconnect(SQLite(), dbname = "example.db")
#' data <- as.matrix(iris[, 1:4])
#' 
#' db_insert_quad(data, "graph", 3, mydb)
#'
#' @export

db_insert_quad <- function(xx, tb_name, chnk_size, db, start = 1) {
  
  insert_chunk_into_db <- function(k) {
    edges <- chunk_builder(xx, k, chnk_size)
    
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
