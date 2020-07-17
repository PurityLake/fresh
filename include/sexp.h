#ifndef __H_FRESH_SEXP__
#define __H_FRESH_SEXP__

#include <stdlib.h>

enum sexp_type {
    SEXP_IDENT = 0,
    SEXP_STRING,
    SEXP_FLOAT,
    SEXP_INT,
    SEXP_LIST,
    SEXP_EMPTY
};

struct sexp_list;

struct sexp {
    union {
        int i;
        float f;
        char *str;
        char *name;
        struct sexp_list *list;
    };
    enum sexp_type type;
};

struct sexp_list {
    struct sexp **sexps;
    size_t length;
};

/**
 * struct sexp_list functions
 */
extern struct sexp_list *create_zero_len_sexp_list();
extern struct sexp_list *create_sexp_list(size_t size);

extern void free_sexp_list(struct sexp_list **list);

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

extern void free_sexp(struct sexp **s);

#endif /* __H_FRESH_SEXP__ */