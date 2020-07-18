#include "fresh/sexp.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * struct sexp_list functions
 */
extern struct sexp_array *create_zero_len_sexp_array() {
    struct sexp_array *list = (struct sexp_array *)malloc(sizeof(struct sexp_array));
    list->sexps = NULL;
    list->length = 0;
    list->num_elems = 0;
    return list;
}

extern struct sexp_array *create_sexp_array(size_t size) {
    struct sexp_array *list = create_zero_len_sexp_array();
    list->sexps = (struct sexp **)malloc(sizeof(struct sexp *) * size);
    list->length = size;
    list->num_elems = 0;
    return list;
}

void add_to_sexp_array(struct sexp_array **array, struct sexp *elem) {
    assert(array != NULL);
    assert(*array != NULL);
    assert(elem != NULL);
    (*array)->num_elems += 1;
    if ((*array)->num_elems > (*array)->length) {
        (*array)->length += 10;
        (*array)->sexps = (struct sexp **)realloc((*array)->sexps, sizeof(struct sexp *) * (*array)->length);
    }
    (*array)->sexps[(*array)->num_elems - 1] = elem;
}

struct sexp *pop_front_sexp_array(struct sexp_array **array) {
    assert(array != NULL);
    assert(*array != NULL);
    if ((*array)->num_elems > 0) {
        struct sexp *elem = (*array)->sexps[0];
        struct sexp **list = (struct sexp **)malloc(sizeof(struct sexp *) * (*array)->length);
        for (size_t i = 1; i < (*array)->num_elems; ++i) {
            list[i-1] = (*array)->sexps[i];
        }
        free((*array)->sexps);
        (*array)->sexps = list;
        (*array)->num_elems -= 1;
        return elem;
    }
    return NULL;
}

BOOL is_empty_sexp_array(struct sexp_array *array) {
    assert(array != NULL);
    return array->num_elems == 0 ? TRUE : FALSE;
}

void free_sexp_array(struct sexp_array **list) {
    assert(list != NULL);
    assert(*list != NULL);
    for (size_t i = 0; i < (*list)->num_elems; ++i) {
        free_sexp(&(*list)->sexps[i]);
        (*list)->sexps[i] = NULL;
    }
    free((*list)->sexps);
    (*list)->sexps = NULL;
    free(*list);
    *list = NULL;
}

/**
 * struct sexp functions
 */
struct sexp *create_empty_sexp() {
    struct sexp *s = (struct sexp *)malloc(sizeof(struct sexp));
    s->type = SEXP_EMPTY;
    return s;
}

struct sexp *create_string_sexp(const char *str) {
    assert(str != NULL);
    struct sexp *s = create_empty_sexp();
    size_t size = strlen(str);
    s->str = (char *)malloc(sizeof(char) * size);
    strcpy(s->str, str);
    s->type = SEXP_STRING;
    return s;
}

struct sexp *create_ident_sexp(const char *name) {
    assert(name != NULL);
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
    s->list = create_zero_len_sexp_array();
    s->type = SEXP_LIST;
    return s;
}

struct sexp *create_list_sexp(size_t size) {
    struct sexp *s = create_empty_sexp();
    s->list = create_sexp_array(size);
    s->type = SEXP_LIST;
    return s;
}

void add_to_list_sexp(struct sexp **s, struct sexp *obj) {
    assert(s != NULL);
    assert(*s != NULL);
    assert(is_list_sexp(*s));
    add_to_sexp_array(&(*s)->list, obj);
}

BOOL is_empty_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_EMPTY ? TRUE : FALSE;
}

BOOL is_string_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_STRING ? TRUE : FALSE;
}

BOOL is_ident_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_IDENT ? TRUE : FALSE;
}
BOOL is_int_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_INT ? TRUE : FALSE;
}
BOOL is_float_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_FLOAT ? TRUE : FALSE;
}
BOOL is_list_sexp(const struct sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_LIST ? TRUE : FALSE;
}
BOOL is_empty_list_sexp(const struct sexp *s) {
    assert(s != NULL);
    if (s->type == SEXP_LIST) {
        return is_empty_sexp_array(s->list);
    }
    return FALSE;
}

void free_sexp(struct sexp **s) {
    assert(s != NULL);
    assert(*s != NULL);
    switch ((*s)->type) {
        case SEXP_LIST:
            free_sexp_array(&(*s)->list);
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