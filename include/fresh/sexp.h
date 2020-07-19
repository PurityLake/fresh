#ifndef __H_FRESH_SEXP__
#define __H_FRESH_SEXP__

#include <stdlib.h>

#include "defines.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

enum SexpType {
    SEXP_IDENT = 0,
    SEXP_STRING,
    SEXP_FLOAT,
    SEXP_INT,
    SEXP_LIST,
    SEXP_EMPTY
};

typedef struct _SexpArray SexpArray;

typedef struct {
    union {
        Int i;
        Float f;
        String str;
        String name;
        SexpArray *list;
    };
    enum SexpType type;
} Sexp;

typedef struct _SexpArray {
    size_t num_elems;
    size_t length;
    Sexp **sexps;
} SexpArray;

/**
 * SexpArray functions
 */
extern Error *create_zero_len_SexpArray(SexpArray **sa);
extern Error *create_SexpArray(SexpArray **sa, size_t size);

extern Error *add_to_SexpArray(SexpArray **array, Sexp *elem);
extern Error *pop_front_SexpArray(SexpArray **array, Sexp **s);

extern BOOL is_empty_SexpArray(SexpArray *list);

extern Error *free_SexpArray(SexpArray **list);

/**
 * Sexp functions
 */
extern Error *create_empty_Sexp(Sexp **s);
extern Error *create_string_Sexp(Sexp **s, const String str);
extern Error *create_ident_Sexp(Sexp **s, const String name);
extern Error *create_int_Sexp(Sexp **s, Int i);
extern Error *create_float_Sexp(Sexp **s, Float f);
extern Error *create_empty_list_Sexp(Sexp **s);
extern Error *create_list_Sexp(Sexp **s, size_t size);

extern Error *add_to_list_Sexp(Sexp **s, Sexp *obj);

extern BOOL is_empty_Sexp(const Sexp *s);
extern BOOL is_string_Sexp(const Sexp *s);
extern BOOL is_ident_Sexp(const Sexp *s);
extern BOOL is_int_Sexp(const Sexp *s);
extern BOOL is_float_Sexp(const Sexp *s);
extern BOOL is_list_Sexp(const Sexp *s);
extern BOOL is_empty_list_Sexp(const Sexp *s);

extern Error *pop_from_front_list_Sexp(Sexp **s, Sexp **out);

extern void free_Sexp(Sexp **s);

#ifdef __cplusplus
}
#endif

#endif /* __H_FRESH_SEXP__ */