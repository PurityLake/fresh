#ifndef __H_FRESH_EVAL__
#define __H_FRESH_EVAL__

#include "sexp.h"

extern Sexp *print_fn(Sexp *rest);
extern Sexp *println_fn(Sexp *rest);
extern Sexp *add_fn(Sexp *rest);

extern Sexp *eval(Sexp *line);

#endif /* __H_FRESH_EVAL__ */