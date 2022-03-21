
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include "narrow.h"
#include "geoarrow.h"

void geoarrow_finalize_array_data(SEXP array_data_xptr) {
    struct ArrowArray* array_data = (struct ArrowArray*) R_ExternalPtrAddr(array_data_xptr);
    if (array_data != nullptr && array_data->release != nullptr) {
        array_data->release(array_data);
    }

    if (array_data != nullptr) {
      free(array_data);
    }
}

void delete_array_view_xptr(SEXP array_view_xptr) {
    geoarrow::ArrayView* array_view =
        reinterpret_cast<geoarrow::ArrayView*>(R_ExternalPtrAddr(array_view_xptr));

    if (array_view != nullptr) {
        delete array_view;
    }
}

void delete_array_builder_xptr(SEXP array_builder_xptr) {
    geoarrow::GeoArrayBuilder* array_builder =
        reinterpret_cast<geoarrow::GeoArrayBuilder*>(R_ExternalPtrAddr(array_builder_xptr));

    if (array_builder != nullptr) {
        delete array_builder;
    }
}