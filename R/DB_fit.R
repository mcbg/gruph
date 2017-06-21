library(gruph)
library(RSQLite)
library(parallel)
library(microbenchmark)

# SET UP -----
#db_path <- "/data/MIBI/graphs/full.db"


db_fit_par <- function(xx, tb_name, chnk_size, start = 1) {

  insert_chunk_into_db <- function(k) {
    edges <- chunk_builder(xx, k, chnk_size)
    dbWriteTable(mydb,
                 tb_name,
                 edges,
                 append = TRUE,
                 row.names = FALSE)
  }

  n <- ncol(xx)
  M <- ceiling(n / chnk_size) # num of chunks
  ch_end <-  M / 2
  k <- start
  
  chs <- 1:ch_end
  cl <- makeCluster(7)

  clusterEvalQ(cl, {
    library(RSQLite)
    library(gruph)
    db_path <- "/data/MIBI/graphs/test2.db"
    mydb <- dbConnect(SQLite(), dbname = db_path)
    RSQLite::dbClearResult(RSQLite::dbSendQuery(mydb, "PRAGMA busy_timeout=5000;"));
    # 2 min bust time
  })

  parLapply(cl, chs, function(k) {
    insert_chunk_into_db(k)
    insert_chunk_into_db(n - k + 1)
    k <- k + 1
  })
  #stopCluster(cl)
  #if (M %% 2 == 1) {
  #  insert_chunk_into_db(ceiling(n / 2))
  #}

}

db_fit <- function(xx, tb_name, chnk_size, db, start = 1) {

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
    insert_chunk_into_db(k)
    insert_chunk_into_db(n - k + 1)
    k <- k + 1
  }

  if (M %% 2 == 1) {
    insert_chunk_into_db(ceiling(n / 2))
  }

}

# DATA ---- 

hippo <- readRDS("data/hippo-may-2017.rds")
ch_size <- 1250

db_path <- "/data/MIBI/graphs/test3.db"
mydb <- dbConnect(SQLite(), dbname = db_path)
db_fit(xx = hippo$expr[, 1:2050],
       tb_name = "edgespar",
       chnk_size = ch_size,
       db = mydb)
