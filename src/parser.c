#include "fresh/parser.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Sexp *s;
    size_t read_to;
} sexp_with_idx;

Error *parse_string(sexp_with_idx **out, String line, int pos) {
    Error *e;
    int idx = 0;
    int capacity = 5;
    String str = malloc(sizeof *str * capacity);
    for (size_t i = pos; i < strlen(line); ++i, ++idx) {
        if (idx >= capacity) {
            capacity += 64;
            str = realloc(str, capacity);
        }
        if (line[i] == '"') {
            if (idx > 0) {
                if (line[idx - 1] == '\\') {
                    str[--idx] = '"';
                } else {
                    (*out)->read_to = i;
                    str = realloc(str, idx);
                    str[idx] = '\0';
                    e = create_string_Sexp(&(*out)->s, str);
                    free_Error(&e);
                    return NoError;
                }
            }
        } else {
            str[idx] = line[i];
        }
    }
    return create_Error(NoObj, "Failed to parse string, missing closing '\"'", 0, pos);
}

Error *translate_to_ident_or_num(Sexp **s, String str) {
    BOOL is_int = FALSE;
    BOOL is_float = FALSE;
    int count_decimal = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if (isdigit(str[i])) {
            if (!is_float) {
                is_int = TRUE;
            }
        } else if (str[i] == '.') {
            is_float = TRUE;
            is_int = FALSE;
            count_decimal++;
            if (count_decimal > 1) {
                is_int = FALSE;
                is_float = FALSE;
                break;
            }
        } else {
            is_int = FALSE;
            is_float = FALSE;
            break;
        }
    }
    Error *e;
    Sexp *out;
    if (is_int) {
        e = create_int_Sexp(&out, atoi(str));
        free_Error(&e);
        add_to_list_Sexp(s, out);
    } else if (is_float) {
        e = create_float_Sexp(&out, atof(str));
        free_Error(&e);
        add_to_list_Sexp(s, out);
    } else {
        e = create_ident_Sexp(&out, str);
        free_Error(&e);
        add_to_list_Sexp(s, out);
    }
    return NoError;
}

Error *parse_Sexp(sexp_with_idx **out, String line, int pos) {
    Sexp *s;
    Error *e = create_list_Sexp(&s, 10);
    free_Error(&e);
    String str = malloc(sizeof *str * 100);
    int idx = 0;
    size_t i;
    for (i = pos; i < strlen(line); ++i) {
        if (line[i] == '(') {
            if (idx > 0) {
                str[idx] = '\0';
                translate_to_ident_or_num(&s, str);
                idx = 0;
            }
            sexp_with_idx *swi = malloc(sizeof *swi);
            Error *e = parse_Sexp(&swi, line, i + 1);
            free_Error(&e);
            i = swi->read_to;
            add_to_list_Sexp(&s, swi->s);
            free(swi);
        } else if (line[i] == ')') {
            if (idx > 0) {
                str[idx] = '\0';
                translate_to_ident_or_num(&s, str);
                idx = 0;
            }
            break;
        } else if (line[i] == '"') {
            sexp_with_idx *swi = malloc(sizeof *swi);
            Error *e = parse_string(&swi, line, i + 1);
            free_Error(&e);
            i = swi->read_to;
            e = add_to_list_Sexp(&s, swi->s);
            free_Error(&e);
            free(swi);
        } else {
            if (strchr(ident_symbols, line[i]) != NULL) {
                str[idx] = line[i];
                ++idx;
            } else if (isspace(line[i])) {
                if (idx > 0) {
                    str[idx] = '\0';
                    translate_to_ident_or_num(&s, str);
                    idx = 0;
                }
            }
        }
    }
    (*out)->read_to = i;
    (*out)->s = s;
    free(str);
    return NoError; 
}

Error *parse_line(Sexp **out, String line) {
    Sexp *s;
    Error *e = create_list_Sexp(&s, 10);
    free_Error(&e);
    BOOL first = TRUE;
    /// TODO: add check
    String str = malloc(sizeof *str * 100);
    int idx = 0;
    for (size_t i = 0; i < strlen(line); ++i) {
        if (line[i] == '(') {
            if (!first) {
                if (idx > 0) {
                    str[idx] = '\0';
                    translate_to_ident_or_num(&s, str);
                    idx = 0;
                }
                sexp_with_idx *swi = malloc(sizeof *swi);
                Error *e = parse_Sexp(&swi, line, i + 1);
                free_Error(&e);
                i = swi->read_to;
                add_to_list_Sexp(&s, swi->s);
                free(swi);
            }
            first = FALSE;
        } else if (line[i] == ')') {
            if (first) {
                *out = NULL;
                return create_Error(NoObj, "First character of an expression must be '(' got ')'", 1, i);
            }
            if (idx > 0) {
                str[idx] = '\0';
                translate_to_ident_or_num(&s, str);
                idx = 0;
            }
            break;
        } else if (line[i] == '"') {
            if (first) {
                *out = NULL;
                return create_Error(NoObj, "First character of an expression must be '(' got '\"'", 1, i);
            }
            sexp_with_idx *swi = malloc(sizeof *swi);
            Error *e = parse_string(&swi, line, i + 1);
            free_Error(&e);
            i = swi->read_to;
            add_to_list_Sexp(&s, swi->s);
            free(swi);
        } else {
            if (first && !isspace(line[i])) {
                *out = NULL;
                char *errorMessage = malloc(sizeof *errorMessage * 128);
                memset(errorMessage, '\0', 128);
                sprintf(errorMessage, "First character of an expression must be '(' got '%c'", line[i]);
                Error *e = create_Error(NoObj, errorMessage, 1, i);
                free(errorMessage);
                return e;
            }
            if (strchr(ident_symbols, line[i]) != NULL) {
                str[idx] = line[i];
                ++idx;
            } else if (isspace(line[i])) {
                if (idx > 0) {
                    str[idx] = '\0';
                    translate_to_ident_or_num(&s, str);
                    idx = 0;
                }
            }
        }
    }
    *out = s;
    free(str);
    return NoError;
}
