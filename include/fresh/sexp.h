#ifndef __H_FRESH_SEXP__
#define __H_FRESH_SEXP__

#include <stdlib.h>

#include"defines.h"

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
extern SexpArray *create_zero_len_SexpArray();
extern SexpArray *create_SexpArray(size_t size);

extern void add_to_SexpArray(SexpArray **array, Sexp *elem);
extern Sexp *pop_front_SexpArray(SexpArray **array);

extern BOOL is_empty_SexpArray(SexpArray *list);

extern void free_SexpArray(SexpArray **list);

/**
 * Sexp functions
 */
extern Sexp *create_empty_sexp();
extern Sexp *create_string_sexp(const String str);
extern Sexp *create_ident_sexp(const String name);
extern Sexp *create_int_sexp(int i);
extern Sexp *create_float_sexp(float f);
extern Sexp *create_empty_list_sexp();
extern Sexp *create_list_sexp(size_t size);

extern void add_to_list_sexp(Sexp **s, Sexp *obj);

extern BOOL is_empty_sexp(const Sexp *s);
extern BOOL is_string_sexp(const Sexp *s);
extern BOOL is_ident_sexp(const Sexp *s);
extern BOOL is_int_sexp(const Sexp *s);
extern BOOL is_float_sexp(const Sexp *s);
extern BOOL is_list_sexp(const Sexp *s);
extern BOOL is_empty_list_sexp(const Sexp *s);

extern void free_sexp(Sexp **s);

#ifdef __cplusplus
}
#endif

#endif /* __H_FRESH_SEXP__ */