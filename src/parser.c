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

sexp_with_idx *parse_string(String line, int pos) {
    int idx = 0;
    int capacity = 5;
    String str = (String )malloc(sizeof(char) * capacity);
    for (size_t i = pos; i < strlen(line); ++i, ++idx) {
        if (idx >= capacity) {
            capacity += 64;
            str = (String )realloc(str, capacity);
        }
        if (line[i] == '"') {
            if (idx > 0) {
                if (line[idx - 1] == '\\') {
                    str[--idx] = '"';
                } else {
                    sexp_with_idx *swi = (sexp_with_idx *)malloc(sizeof(sexp_with_idx));
                    swi->read_to = i;
                    str = (String )realloc(str, idx);
                    str[idx] = '\0';
                    swi->s = create_string_Sexp(str);
                    free(str);
                    return swi;
                }
            }
        } else {
            str[idx] = line[i];
        }
    }
}

void translate_to_ident_or_num(Sexp **s, String str) {
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
            break;
        }
    }
    if (is_int) {
        add_to_list_Sexp(s, create_int_Sexp(atoi(str)));
    } else if (is_float) {
        add_to_list_Sexp(s, create_float_Sexp(atof(str)));
    } else {
        add_to_list_Sexp(s, create_ident_Sexp(str));
    }
}

sexp_with_idx *parse_Sexp(String line, int pos) {
    Sexp *s = create_list_Sexp(10);
    String str = (String )malloc(sizeof(char) * 100);
    int idx = 0;
    size_t i;
    for (i = pos; i < strlen(line); ++i) {
        if (line[i] == '(') {
            if (idx > 0) {
                str[idx] = '\0';
                translate_to_ident_or_num(&s, str);
                idx = 0;
            }
            sexp_with_idx *swi = parse_Sexp(line, i + 1);
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
            sexp_with_idx *swi = parse_string(line, i + 1);
            i = swi->read_to;
            add_to_list_Sexp(&s, swi->s);
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
    sexp_with_idx *swi = (sexp_with_idx *)malloc(sizeof(sexp_with_idx));
    swi->read_to = i;
    swi->s = s;
    free(str);
    return swi; 
}

Sexp *parse_line(String line) {
    Sexp *s = create_list_Sexp(10);
    BOOL first = TRUE;
    String str = (String )malloc(sizeof(char) * 100);
    int idx = 0;
    for (size_t i = 0; i < strlen(line); ++i) {
        if (line[i] == '(') {
            if (!first) {
                if (idx > 0) {
                    str[idx] = '\0';
                    translate_to_ident_or_num(&s, str);
                    idx = 0;
                }
                sexp_with_idx *swi = parse_Sexp(line, i + 1);
                i = swi->read_to;
                add_to_list_Sexp(&s, swi->s);
                free(swi);
            }
            first = FALSE;
        } else if (line[i] == ')') {
            if (idx > 0) {
                str[idx] = '\0';
                translate_to_ident_or_num(&s, str);
                idx = 0;
            }
            break;
        } else if (line[i] == '"') {
            sexp_with_idx *swi = parse_string(line, i + 1);
            i = swi->read_to;
            add_to_list_Sexp(&s, swi->s);
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
    free(str);
    return s;
}