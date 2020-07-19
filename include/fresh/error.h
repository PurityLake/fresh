#ifndef __H_FRESH_ERROR__
#define __H_FRESH_ERROR__

#include "defines.h"

#include <stddef.h>

#define NoObj NULL
#define NoError NULL

typedef struct {
    void *obj;
    char *message;
    int lineno;
    int col;
} Error;

extern Error *create_Error(void *s, const char *msg, int lineno, int col);
extern Error *free_Error(Error **e);

inline static BOOL is_Error(Error *e) { return e != NoError; }
inline static BOOL is_not_Error(Error *e) { return e == NoError; }

#endif /* __H_FRESH_ERROR__ */