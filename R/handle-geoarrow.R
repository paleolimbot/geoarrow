
#' Handle Arrow arrays
#'
#' @inheritParams wk::wk_handle
#' @param geoarrow_schema Override the `schema` of the array stream
#'   (e.g., to provide geo metadata).
#' @param geoarrow_n_features Manually specify the number of features
#'   when reading a stream if this value is known (or `NA_integer`
#'   if it is not).
#'
#' @return The result of `handler`
#' @export
#' @importFrom wk wk_handle
#'
wk_handle.narrow_array <- function(handleable, handler, ...) {
  handler <- wk::as_wk_handler(handler)
  metadata <- handleable$schema$metadata
  extension <- scalar_chr(metadata[["ARROW:extension:name"]])
  geo_metadata <- geoarrow_metadata(handleable$schema)

  switch(
    extension,
    "geoarrow.wkt" = ,
    "geoarrow.wkb" = ,
    "geoarrow.point" = ,
    "geoarrow.linestring" = ,
    "geoarrow.polygon" = ,
    "geoarrow.multipoint" = ,
    "geoarrow.multilinestring" = ,
    "geoarrow.multipolygon" = handle_geoarrow_wk(handleable, handler),
    stop(sprintf("Unsupported extension type '%s'", extension), call. = FALSE)
  )
}

#' @export
#' @rdname wk_handle.narrow_array
wk_handle.narrow_array_stream <- function(handleable, handler, ...,
                                          geoarrow_schema = narrow::narrow_array_stream_get_schema(handleable),
                                          geoarrow_n_features = NA_integer_) {
  handler <- wk::as_wk_handler(handler)
  metadata <- geoarrow_schema$metadata
  extension <- scalar_chr(metadata[["ARROW:extension:name"]])
  geo_metadata <- geoarrow_metadata(geoarrow_schema)

  switch(
    extension,
    "geoarrow.point" = ,
    "geoarrow.wkb" = ,
    "geoarrow.wkt" = ,
    "geoarrow.linestring" = ,
    "geoarrow.polygon" = ,
    "geoarrow.multipoint" = ,
    "geoarrow.multilinestring" = ,
    "geoarrow.multipolygon" = handle_geoarrow_stream_wk(handleable, handler, geoarrow_schema, geoarrow_n_features),
    stop(sprintf("Unsupported extension type '%s'", extension), call. = FALSE)
  )
}

#' @export
#' @rdname wk_handle.narrow_array
wk_handle.geoarrow_vctr <- function(handleable, handler, ...) {
  .Call(
    geoarrow_c_handle_vctr,
    handleable,
    wk::as_wk_handler(handler)
  )
}

handle_geoarrow_wk <- function(array, handler) {
  handle_geoarrow_stream_wk(
    narrow::as_narrow_array_stream(array),
    handler,
    array$schema,
    n_features = array$array_data$length
  )
}

handle_geoarrow_stream_wk <- function(array_stream, handler,
                                      schema = narrow::narrow_array_stream_get_schema(array_stream),
                                      n_features = NA_integer_) {
  .Call(geoarrow_c_handle_stream, list(array_stream, schema, n_features), wk::as_wk_handler(handler))
}

# for testing
geoarrow_create_wkt <- function(x, ...) {
  array <- narrow::as_narrow_array(x)
  array$schema <- geoarrow_schema_wkt(...)
  array
}

geoarrow_create_wkb <- function(x, ...) {
  lens <- lengths(unclass(x))
  data <- unlist(x, use.names = FALSE)
  validity <- if (any(is.na(x))) narrow::as_narrow_bitmask(!is.na(x))

  narrow::narrow_array(
    schema = geoarrow_schema_wkb(...),
    array_data = narrow::narrow_array_data(
      length = length(x),
      null_count = sum(is.na(x)),
      buffers = list(
        validity,
        as.integer(c(0L, cumsum(lens))),
        data
      )
    )
  )
}
