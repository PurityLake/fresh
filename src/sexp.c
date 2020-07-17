#include "sexp.h"

#include <string.h>

extern struct sexp_list *create_zero_len_sexp_list() {
    struct sexp_list *list = (struct sexp_list *)malloc(sizeof(struct sexp_list));
    list->sexps = NULL;
    list->length = 0;
    return list;
}

extern struct sexp_list *create_sexp_list(size_t size) {
    struct sexp_list *list = create_zero_len_sexp_list();
    list->sexps = (struct sexp **)malloc(sizeof(struct sexp *) * size);
    list->length = size;
    return list;
}

struct sexp *create_empty_sexp() {
    struct sexp *s = (struct sexp *)malloc(sizeof(struct sexp));
    s->type = SEXP_EMPTY;
    return s;
}

struct sexp *create_string_sexp(const char *str) {
    struct sexp *s = create_empty_sexp();
    size_t size = strlen(str);
    s->str = (char *)malloc(sizeof(char) * size);
    strcpy(s->str, str);
    s->type = SEXP_STRING;
    return s;
}

struct sexp *create_ident_sexp(const char *name) {
    struct sexp *s = create_empty_sexp();
    size_t size = strlen(name);
    s->name = (char *)malloc(sizeof(char) * size);
    strcpy(s->name, name);
    s->type = SEXP_IDENT;
    return s;
}

struct sexp *create_int_sexp(int i) {
    struct sexp *s = create_empty_sexp();
    s->i = i;
    s->type = SEXP_INT;
    return s;
}

struct sexp *create_float_sexp(float f) {
    struct sexp *s = create_empty_sexp();
    s->f = f;
    s->type = SEXP_FLOAT;
    return s;
}

struct sexp *create_empty_list_sexp() {
    struct sexp *s = create_empty_sexp();
    s->list = create_zero_len_sexp_list();
    s->type = SEXP_LIST;
    return s;
}

struct sexp *create_list_sexp(size_t size) {
    struct sexp *s = create_empty_sexp();
    s->list = create_sexp_list(size);
    s->type = SEXP_LIST;
    return s;
}

void free_sexp(struct sexp **s) {
    if (s != NULL && *s != NULL) {
        switch ((*s)->type) {
            case SEXP_LIST:
                free_sexp_list(&(*s)->list);
                break;
            case SEXP_STRING:
                free((*s)->str);
                break;
            case SEXP_IDENT:
                free((*s)->name);
                break;
            default:
                break;
        }
        free(*s);
        *s = NULL;
    }
}

void free_sexp_list(struct sexp_list **list) {
    if (list != NULL && *list != NULL) {
        for (int i = 0; i < (*list)->length; ++i) {
            free_sexp(&(*list)->sexps[i]);
            (*list)->sexps[i] = NULL;
        }
        free((*list)->sexps);
        (*list)->sexps = NULL;
        free(*list);
        *list = NULL;
    }
}