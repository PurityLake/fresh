#ifndef __H_FRESH_EVAL__
#define __H_FRESH_EVAL__

#include "error.h"
#include "sexp.h"

extern Error *print_fn(Sexp *rest, Sexp **out);
extern Error *println_fn(Sexp *rest, Sexp **out);
extern Error *add_fn(Sexp *rest, Sexp **out);

extern Error *eval(Sexp *line, Sexp **out);

#endif /* __H_FRESH_EVAL__ */