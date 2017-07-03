calc_pval <- function(edges, N) {
    if (!requireNamespace("ggplot2", quietly = TRUE)) {
          stop("ggplot2 needed for this function to work. ",
                        call. = FALSE)
  }
  dfs <- edges$df 
    ws <- edges$weights
    get_p_value <- function(w, degrees) pchisq(2 * w, degrees, lower.tail = TRUE)
      unlist(Map(get_p_value, ws, dfs))
} 

pval_dataframe <- function(edges, N) {
    pvals <- calc_pval(edges,N)
  pvals <- pvals[order(-pvals)]
    
    data.frame(index = (1:length(pvals))/length(pvals),
                            p = pvals)
}

weight_plot <- function(edges, N) {
    if (!requireNamespace("ggplot2", quietly = TRUE)) {
          stop("ggplot2 needed for this function to work. ",
                        call. = FALSE)
  }
  # N: number of observations
  
  xx <- pval_dataframe(edges, N)
    
    ggplot(aes(x = index, y = p), data = xx) +
        geom_line() 
    #geom_abline(slope = -1, intercept = 1, col = "red", size = 1.2) 
    #ylim(0, 1)
    #facet_wrap(~df) #+ xlim(0, 1) + ylim(0, 1) 
    # histogram:
    #geom_histogram(binwidth = 0.1) + 
    #geom_hline(0.1) +
}

weight_histogram <- function(edges, N, facet = TRUE){
    if (!requireNamespace("ggplot2", quietly = TRUE)) {
          stop("ggplot2 needed for this function to work. ",
                        call. = FALSE)
  }
  # N: number of observations
  
  get_p_value <- function(w, degrees) pchisq(2 * N * w, degrees, lower.tail = TRUE)
    w <- attr(edges, "weights")
    dfs <- attr(edges, "df")
      pvals <- unlist(Map(get_p_value, w, dfs))
      pvals[order]
        
        xx <- data.frame(index = (1:length(pvals))/length(pvals),
                                            p = pvals)
        
        p <- ggplot(aes(x = p, col = df), data = xx) +
            geom_histogram(binwidth = 0.1)
          if (facet)
                p <- p + facet_wrap(~df)
            p
}

