powerpc_apple <- grepl("powerpc-apple-darwin", R.version$platform)

safe_expect_error <- function(..., msg) {
  os.type <- Sys.info()["sysname"]
  if (os.type == "Darwin") {
    expect_error(...)
  } else {
    expect_error(..., msg)
  }
}
