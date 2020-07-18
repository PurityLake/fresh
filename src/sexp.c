#include "fresh/sexp.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * sexp_list functions
 */
extern SexpArray *create_zero_len_sexp_array() {
    SexpArray *list = (SexpArray *)malloc(sizeof(SexpArray));
    list->sexps = NULL;
    list->length = 0;
    list->num_elems = 0;
    return list;
}

extern SexpArray *create_sexp_array(size_t size) {
    SexpArray *list = create_zero_len_sexp_array();
    list->sexps = (Sexp **)malloc(sizeof(Sexp *) * size);
    list->length = size;
    list->num_elems = 0;
    return list;
}

void add_to_sexp_array(SexpArray **array, Sexp *elem) {
    assert(array != NULL);
    assert(*array != NULL);
    assert(elem != NULL);
    (*array)->num_elems += 1;
    if ((*array)->num_elems > (*array)->length) {
        (*array)->length += 10;
        (*array)->sexps = (Sexp **)realloc((*array)->sexps, sizeof(Sexp *) * (*array)->length);
    }
    (*array)->sexps[(*array)->num_elems - 1] = elem;
}

Sexp *pop_front_sexp_array(SexpArray **array) {
    assert(array != NULL);
    assert(*array != NULL);
    if ((*array)->num_elems > 0) {
        Sexp *elem = (*array)->sexps[0];
        Sexp **list = (Sexp **)malloc(sizeof(Sexp *) * (*array)->length);
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

BOOL is_empty_sexp_array(SexpArray *array) {
    assert(array != NULL);
    return array->num_elems == 0 ? TRUE : FALSE;
}

void free_sexp_array(SexpArray **list) {
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
 * Sexp functions
 */
Sexp *create_empty_sexp() {
    Sexp *s = (Sexp *)malloc(sizeof(Sexp));
    s->type = SEXP_EMPTY;
    return s;
}

Sexp *create_string_sexp(const String str) {
    assert(str != NULL);
    Sexp *s = create_empty_sexp();
    size_t size = strlen(str);
    s->str = (String )malloc(sizeof(char) * size);
    strcpy(s->str, str);
    s->type = SEXP_STRING;
    return s;
}

Sexp *create_ident_sexp(const String name) {
    assert(name != NULL);
    Sexp *s = create_empty_sexp();
    size_t size = strlen(name);
    s->name = (String )malloc(sizeof(char) * size);
    strcpy(s->name, name);
    s->type = SEXP_IDENT;
    return s;
}

Sexp *create_int_sexp(int i) {
    Sexp *s = create_empty_sexp();
    s->i = i;
    s->type = SEXP_INT;
    return s;
}

Sexp *create_float_sexp(float f) {
    Sexp *s = create_empty_sexp();
    s->f = f;
    s->type = SEXP_FLOAT;
    return s;
}

Sexp *create_empty_list_sexp() {
    Sexp *s = create_empty_sexp();
    s->list = create_zero_len_sexp_array();
    s->type = SEXP_LIST;
    return s;
}

Sexp *create_list_sexp(size_t size) {
    Sexp *s = create_empty_sexp();
    s->list = create_sexp_array(size);
    s->type = SEXP_LIST;
    return s;
}

void add_to_list_sexp(Sexp **s, Sexp *obj) {
    assert(s != NULL);
    assert(*s != NULL);
    assert(is_list_sexp(*s));
    add_to_sexp_array(&(*s)->list, obj);
}

BOOL is_empty_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_EMPTY ? TRUE : FALSE;
}

BOOL is_string_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_STRING ? TRUE : FALSE;
}

BOOL is_ident_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_IDENT ? TRUE : FALSE;
}
BOOL is_int_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_INT ? TRUE : FALSE;
}
BOOL is_float_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_FLOAT ? TRUE : FALSE;
}
BOOL is_list_sexp(const Sexp *s) {
    assert(s != NULL);
    return s->type == SEXP_LIST ? TRUE : FALSE;
}
BOOL is_empty_list_sexp(const Sexp *s) {
    assert(s != NULL);
    if (s->type == SEXP_LIST) {
        return is_empty_sexp_array(s->list);
    }
    return FALSE;
}

void free_sexp(Sexp **s) {
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