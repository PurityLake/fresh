#ifndef __H_FRESH_SEXP__
#define __H_FRESH_SEXP__

#include <stdlib.h>

#include"defines.h"

enum sexp_type {
    SEXP_IDENT = 0,
    SEXP_STRING,
    SEXP_FLOAT,
    SEXP_INT,
    SEXP_LIST,
    SEXP_EMPTY
};

struct sexp_array;

struct sexp {
    union {
        int i;
        float f;
        char *str;
        char *name;
        struct sexp_array *list;
    };
    enum sexp_type type;
};

struct sexp_array {
    struct sexp **sexps;
    size_t num_elems;
    size_t length;
};

/**
 * struct sexp_array functions
 */
extern struct sexp_array *create_zero_len_sexp_array();
extern struct sexp_array *create_sexp_array(size_t size);

extern void add_to_sexp_array(struct sexp_array **array, struct sexp *elem);
extern struct sexp *pop_front_sexp_array(struct sexp_array **array);

extern BOOL is_empty_sexp_array(struct sexp_array *list);

extern void free_sexp_array(struct sexp_array **list);

/**
 * struct sexp functions
 */
extern struct sexp *create_empty_sexp();
extern struct sexp *create_string_sexp(const char *str);
extern struct sexp *create_ident_sexp(const char *name);
extern struct sexp *create_int_sexp(int i);
extern struct sexp *create_float_sexp(float f);
extern struct sexp *create_empty_list_sexp();
extern struct sexp *create_list_sexp(size_t size);

extern BOOL is_empty_sexp(const struct sexp *s);
extern BOOL is_string_sexp(const struct sexp *s);
extern BOOL is_ident_sexp(const struct sexp *s);
extern BOOL is_int_sexp(const struct sexp *s);
extern BOOL is_float_sexp(const struct sexp *s);
extern BOOL is_list_sexp(const struct sexp *s);
extern BOOL is_empty_list_sexp(const struct sexp *s);

extern void free_sexp(struct sexp **s);

#endif /* __H_FRESH_SEXP__ */