#ifndef __H_FRESH_SEXP__
#define __H_FRESH_SEXP__

#include <stdlib.h>

#include"defines.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sexp_type {
    SEXP_IDENT = 0,
    SEXP_STRING,
    SEXP_FLOAT,
    SEXP_INT,
    SEXP_LIST,
    SEXP_EMPTY
};

typedef struct _sexp_array sexp_array;

typedef struct {
    union {
        int i;
        float f;
        char *str;
        char *name;
        sexp_array *list;
    };
    enum sexp_type type;
} sexp;

typedef struct _sexp_array {
    size_t num_elems;
    size_t length;
    sexp **sexps;
} sexp_array;

/**
 * sexp_array functions
 */
extern sexp_array *create_zero_len_sexp_array();
extern sexp_array *create_sexp_array(size_t size);

extern void add_to_sexp_array(sexp_array **array, sexp *elem);
extern sexp *pop_front_sexp_array(sexp_array **array);

extern BOOL is_empty_sexp_array(sexp_array *list);

extern void free_sexp_array(sexp_array **list);

/**
 * sexp functions
 */
extern sexp *create_empty_sexp();
extern sexp *create_string_sexp(const char *str);
extern sexp *create_ident_sexp(const char *name);
extern sexp *create_int_sexp(int i);
extern sexp *create_float_sexp(float f);
extern sexp *create_empty_list_sexp();
extern sexp *create_list_sexp(size_t size);

extern void add_to_list_sexp(sexp **s, sexp *obj);

extern BOOL is_empty_sexp(const sexp *s);
extern BOOL is_string_sexp(const sexp *s);
extern BOOL is_ident_sexp(const sexp *s);
extern BOOL is_int_sexp(const sexp *s);
extern BOOL is_float_sexp(const sexp *s);
extern BOOL is_list_sexp(const sexp *s);
extern BOOL is_empty_list_sexp(const sexp *s);

extern void free_sexp(sexp **s);

#ifdef __cplusplus
}
#endif

#endif /* __H_FRESH_SEXP__ */