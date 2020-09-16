#include "fresh/sexp.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * sexp_list functions
 */
Error *create_zero_len_SexpArray(SexpArray **sa) {
    *sa = malloc(sizeof **sa);
    if (*sa == NULL) {
        return create_Error(NoObj, "Failed to allocate memory in create_zero_len_SexpArray: " __FILE__, __LINE__, 0);
    }
    (*sa)->sexps = NULL;
    (*sa)->length = 0;
    (*sa)->num_elems = 0;
    return NoError;
}

Error *create_SexpArray(SexpArray **sa, size_t size) {
    Error *e = create_zero_len_SexpArray(sa);
    if (is_Error(e)) {
        return e;
    }
    (*sa)->sexps = malloc(sizeof(*(*sa)->sexps) * size);
    if ((*sa)->sexps == NULL) {
        return create_Error(NoObj, "Failed to allocate memory in create_SexpArray: " __FILE__, __LINE__, 0);
    }
    (*sa)->length = size;
    (*sa)->num_elems = 0;
    return NoError;
}

Error *add_to_SexpArray(SexpArray **array, Sexp *elem) {
    if (array == NULL || *array == NULL || elem == NULL) {
        return create_Error(NoObj, "NULL passed to add_to_SexpArray: " __FILE__, __LINE__, 0);
    }
    (*array)->num_elems += 1;
    if ((*array)->num_elems > (*array)->length) {
        (*array)->length += 10;
        (*array)->sexps = realloc((*array)->sexps, sizeof(Sexp*) * (*array)->length);
        if ((*array)->sexps == NULL) {
            return create_Error(NoObj, "Failed to reallocate memory for in add_to_SexpArray: " __FILE__, __LINE__, 0);
        }
    }
    (*array)->sexps[(*array)->num_elems - 1] = elem;
    return NoError;
}

Error *pop_front_SexpArray(SexpArray **array, Sexp **s) {
    if (array == NULL || *array == NULL || s == NULL) {
        return create_Error(NoObj, "NULL passed to pop_front_SexpArray: " __FILE__, __LINE__, 0);
    }
    if ((*array)->num_elems > 0) {
        *s = (*array)->sexps[0];
        Sexp **list = malloc(sizeof *list * (*array)->length);
        if (list == NULL) {
            return create_Error(NoObj, "Failed to allocate memory pop_front_SexpAray: " __FILE__, __LINE__, 0);
        }
        for (size_t i = 1; i < (*array)->num_elems; ++i) {
            list[i-1] = (*array)->sexps[i];
        }
        free((*array)->sexps);
        (*array)->sexps = list;
        (*array)->num_elems -= 1;
    } else {
        *s = NULL;
    }
    return NoError;
}

BOOL is_empty_SexpArray(SexpArray *array) {
    return array != NULL && array->num_elems == 0 ? TRUE : FALSE;
}

Error *free_SexpArray(SexpArray **list) {
    if (list == NULL || *list == NULL) return NoError; 
    for (size_t i = 0; i < (*list)->num_elems; ++i) {
        free_Sexp(&(*list)->sexps[i]);
        (*list)->sexps[i] = NULL;
    }
    free((*list)->sexps);
    (*list)->sexps = NULL;
    free(*list);
    *list = NULL;
    return NoError;
}

/**
 * Sexp functions
 */
Error *create_empty_Sexp(Sexp **s) {
    if (s == NULL) {
        return create_Error(NoObj, "NULL passed to create_empty_Sexp: " __FILE__, __LINE__, 0);
    }
    *s = malloc(sizeof **s);
    if (*s == NULL) {
        return create_Error(NoObj, "Failed to allocate memory in create_empty_Sexp: " __FILE__, __LINE__, 0);
    }
    (*s)->type = SEXP_EMPTY;
    return NoError;
}

Error *create_string_Sexp(Sexp **s, const String str) {
    if (s == NULL || str == NULL) {
        return create_Error(NoObj, "NULL passed to create_string_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    size_t size = strlen(str) + 1;
    (*s)->str = malloc(sizeof(char) * size);
    if ((*s)->str == NULL) {
        return create_Error(NoObj, "Failed to allocate memory in create_string_Sexp: " __FILE__, __LINE__, 0);
    }
    strcpy((*s)->str, str);
    (*s)->type = SEXP_STRING;
    return NoError;
}

Error *create_ident_Sexp(Sexp **s, const String name) {
    if (s == NULL || name == NULL) {
        return create_Error(NoObj, "NULL passed to create_ident_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    size_t size = strlen(name) + 1;
    (*s)->name = malloc(sizeof(char) * size);
    if ((*s)->name == NULL) {
        return create_Error(NoObj, "Failed to allocate memory in create_string_Sexp: " __FILE__, __LINE__, 0);
    }
    strcpy((*s)->name, name);
    (*s)->type = SEXP_IDENT;
    return NoError;
}

Error *create_int_Sexp(Sexp **s, Int i) {
    if (s == NULL) {
        return create_Error(NoObj, "NULL passed to create_int_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    (*s)->i = i;
    (*s)->type = SEXP_INT;
    return NoError;
}

Error *create_float_Sexp(Sexp **s, Float f) {
    if (s == NULL) {
        return create_Error(NoObj, "NULL passed to create_float_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    (*s)->f = f;
    (*s)->type = SEXP_FLOAT;
    return NoError;
}

Error *create_empty_list_Sexp(Sexp **s) {
    if (s == NULL) {
        return create_Error(NoObj, "NULL passed to create_empty_list_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    e = create_zero_len_SexpArray(&(*s)->list);
    free_Error(&e);
    (*s)->type = SEXP_LIST;
    return NoError;
}

Error *create_list_Sexp(Sexp **s, size_t size) {
    if (s == NULL) {
        return create_Error(NoObj, "NULL passed to create_list_Sexp: " __FILE__, __LINE__, 0);
    }
    Error *e = create_empty_Sexp(s);
    free_Error(&e);
    e = create_SexpArray(&(*s)->list, size);
    free_Error(&e);
    (*s)->type = SEXP_LIST;
    return NoError;
}

Error *add_to_list_Sexp(Sexp **s, Sexp *obj) {
    if (s == NULL || *s == NULL || obj == NULL) {
        return create_Error(NoObj, "NULL passed to add_to_Sexp: " __FILE__, __LINE__, 0);
    }
    if (!is_list_Sexp(*s)) {
        return create_Error(NoObj, "add_to_list_Sexp requires a list: " __FILE__, __LINE__, 0);
    }
    Error *e = add_to_SexpArray(&(*s)->list, obj);
    free_Error(&e);
    return NoError;
}

BOOL is_empty_Sexp(const Sexp *s) {
    return s->type == SEXP_EMPTY ? TRUE : FALSE;
}

BOOL is_string_Sexp(const Sexp *s) {
    return s->type == SEXP_STRING ? TRUE : FALSE;
}

BOOL is_ident_Sexp(const Sexp *s) {
    return s->type == SEXP_IDENT ? TRUE : FALSE;
}

BOOL is_int_Sexp(const Sexp *s) {
    return s->type == SEXP_INT ? TRUE : FALSE;
}

BOOL is_float_Sexp(const Sexp *s) {
    return s->type == SEXP_FLOAT ? TRUE : FALSE;
}

BOOL is_list_Sexp(const Sexp *s) {
    return s->type == SEXP_LIST ? TRUE : FALSE;
}

BOOL is_empty_list_Sexp(const Sexp *s) {
    if (s->type == SEXP_LIST) {
        return is_empty_SexpArray(s->list);
    }
    return FALSE;
}

Error *pop_from_front_list_Sexp(Sexp **s, Sexp **out){
    if (s == NULL || *s == NULL || out == NULL) {
        return create_Error(NoObj, "NULL passed to add_to_Sexp: " __FILE__, __LINE__, 0);
    }
    if (!is_list_Sexp(*s)) {
        return create_Error(NoObj, "pop_from_front_list_Sexp requires a list: " __FILE__, __LINE__, 0);
    }
    Error *e = pop_front_SexpArray(&(*s)->list, out);
    free_Error(&e);
    return NoError;
}

void free_Sexp(Sexp **s) {
    if (s != NULL && *s != NULL) {
        switch ((*s)->type) {
            case SEXP_LIST:
                free_SexpArray(&(*s)->list);
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
    }
}
