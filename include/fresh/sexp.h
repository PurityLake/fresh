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
extern Sexp *create_empty_Sexp();
extern Sexp *create_string_Sexp(const String str);
extern Sexp *create_ident_Sexp(const String name);
extern Sexp *create_int_Sexp(int i);
extern Sexp *create_float_Sexp(float f);
extern Sexp *create_empty_list_Sexp();
extern Sexp *create_list_Sexp(size_t size);

extern void add_to_list_Sexp(Sexp **s, Sexp *obj);

extern BOOL is_empty_Sexp(const Sexp *s);
extern BOOL is_string_Sexp(const Sexp *s);
extern BOOL is_ident_Sexp(const Sexp *s);
extern BOOL is_int_Sexp(const Sexp *s);
extern BOOL is_float_Sexp(const Sexp *s);
extern BOOL is_list_Sexp(const Sexp *s);
extern BOOL is_empty_list_Sexp(const Sexp *s);

extern void free_Sexp(Sexp **s);

#ifdef __cplusplus
}
#endif

#endif /* __H_FRESH_SEXP__ */