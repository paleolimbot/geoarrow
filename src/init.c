#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

SEXP geoarrow_c_handle_wk(SEXP data, SEXP handler_xptr);
SEXP geoarrow_c_compute_handler_new(SEXP op_sexp, SEXP schema_xptr, SEXP array_sexp_out);
SEXP geoarrow_c_compute(SEXP array_from_sexp, SEXP array_to_sexp);

static const R_CallMethodDef CallEntries[] = {
    {"geoarrow_c_handle_wk", (DL_FUNC) &geoarrow_c_handle_wk, 2},
    {"geoarrow_c_compute_handler_new", (DL_FUNC) &geoarrow_c_compute_handler_new, 3},
    {"geoarrow_c_compute", (DL_FUNC) &geoarrow_c_compute, 3},
    {NULL, NULL, 0}
};

void R_init_geoarrow(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
