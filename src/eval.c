#include "fresh/eval.h"

#include <stdio.h>
#include <string.h>

Sexp *print_fn(Sexp *rest) {
    if (!is_list_Sexp(rest)) {
        switch (rest->type) {
            case SEXP_STRING:
                printf("%s ", rest->str);
                break;
            case SEXP_IDENT:
                printf("%s ", rest->name);
                break;
            case SEXP_INT:
                printf("%ld ", rest->i);
                break;
            case SEXP_FLOAT:
                printf("%f ", rest->f);
                break;
        }
    } else {
        Sexp *curr;
        while ((curr = pop_from_front_list_Sexp(&rest)) != NULL) {
            switch (curr->type) {
                case SEXP_STRING:
                    printf("%s ", curr->str);
                    break;
                case SEXP_IDENT:
                    printf("%s ", curr->name);
                    break;
                case SEXP_INT:
                    printf("%ld ", curr->i);
                    break;
                case SEXP_FLOAT:
                    printf("%f ", curr->f);
                    break;
                case SEXP_LIST:
                    print_fn(eval(curr));
                    break;
            }
        }
    }
    return create_empty_Sexp();
}

Sexp *println_fn(Sexp *rest) {
    if (!is_list_Sexp(rest)) {
        print_fn(rest);
    } else {
        Sexp *curr;
        while ((curr = pop_from_front_list_Sexp(&rest)) != NULL) {
            if (is_list_Sexp(curr)) {
                print_fn(eval(curr));
            } else {
                print_fn(curr);
            }
        }
    }
    printf("\n");
    return create_empty_Sexp();
}

Sexp *add_fn(Sexp *rest) {
    Sexp *curr;
    BOOL is_float = FALSE;
    Int int_ret = 0;
    Float float_ret = 0;
    while ((curr = pop_from_front_list_Sexp(&rest)) != NULL) {
        switch (curr->type) {
            case SEXP_INT:
                if (is_float) {
                    float_ret += (Float)curr->i;
                } else {
                    int_ret += curr->i;
                }
                break;
            case SEXP_FLOAT:
                if (!is_float) {
                    float_ret = (Float)int_ret;
                    is_float = TRUE;
                }
                float_ret += curr->f;
                break;
            case SEXP_LIST:
                {
                    Sexp *val = eval(curr);
                    if (is_int_Sexp(val)) {
                        if (is_float) {
                            float_ret += (Float)val->i;
                        } else {
                            int_ret += val->i;
                        }
                    } else if (is_float_Sexp(val)) {
                        if (!is_float) {
                            float_ret = (Float)int_ret;
                            is_float = TRUE;
                        }
                        float_ret += val->f;
                    }
                }
                break;
        }
    }
    if (is_float) {
        return create_float_Sexp(float_ret);
    } else {
        return create_int_Sexp(int_ret);
    }
}

Sexp *eval(Sexp *line) {
    Sexp *front = pop_from_front_list_Sexp(&line);
    switch (front->type) {
        case SEXP_IDENT:
            if (strcmp(front->name, "println") == 0) {
                return println_fn(line);
            } else if(strcmp(front->name, "print") == 0) {
                return print_fn(line);
            } else if(strcmp(front->name, "+") == 0) {
                return add_fn(line);
            }
            break;
    }
    return NULL;
}