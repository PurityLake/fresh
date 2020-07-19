#include "fresh/error.h"
#include "fresh/sexp.h"

#include <string.h>

Error *create_Error(void *s, const char *msg, int lineno, int col) {
    Error *e = (Error *)malloc(sizeof(Error));
    e->obj = s;
    
    size_t len = strlen(msg);
    e->message = (char *)malloc(sizeof(char) * len);
    strcpy(e->message, msg);

    e->lineno = lineno;
    e->col = col;

    return e;
}

Error *free_Error(Error **e) {
    if (e != NULL && *e != NULL) {
        Sexp *s = (Sexp *)(*e)->obj;
        free_Sexp(&s);
        free((*e)->message);
        free(*e);
    }
    return NoError;
}