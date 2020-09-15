#ifndef __H_FRESH_EVAL__
#define __H_FRESH_EVAL__

#include "error.h"
#include "scope.h"
#include "sexp.h"

extern Error *print_fn(Scope **s, Sexp *rest, Sexp **out);
extern Error *println_fn(Scope **s, Sexp *rest, Sexp **out);
extern Error *add_fn(Scope **s, Sexp *rest, Sexp **out);

extern Error *eval(Scope **s, Sexp *line, Sexp **out);

#endif /* __H_FRESH_EVAL__ */
