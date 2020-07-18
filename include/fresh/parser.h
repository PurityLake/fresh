#ifndef __H_FRESH_PARSER__
#define __H_FRESH_PARSER__

#include "sexp.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *ident_symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345678?!<>;:/.";

extern Sexp *parse_line(String line);

#ifdef __cplusplus
}
#endif

#endif /* __H_FRESH_PARSER__ */