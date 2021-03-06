#include "fresh/eval.h"

#include <stdio.h>
#include <string.h>

Error *print_fn(Scope **s, Sexp *rest, Sexp **out) {
    if (!is_list_Sexp(rest)) {
        switch (rest->type) {
            case SEXP_STRING:
                printf("%s ", rest->str);
                break;
            case SEXP_IDENT:
                printf("%s ", rest->name);
                break;
            case SEXP_INT:
                printf("%lld ", rest->i);
                break;
            case SEXP_FLOAT:
                printf("%f ", rest->f);
                break;
            default:
                break;
        }
    } else {
        Sexp *curr = { 0 };
        Error *e = pop_from_front_list_Sexp(&rest, &curr);
        free_Error(&e);
        while(curr != NULL) {
            switch (curr->type) {
                case SEXP_STRING:
                    printf("%s ", curr->str);
                    break;
                case SEXP_IDENT:
                    printf("%s ", curr->name);
                    break;
                case SEXP_INT:
                    printf("%lld ", curr->i);
                    break;
                case SEXP_FLOAT:
                    printf("%f ", curr->f);
                    break;
                case SEXP_LIST:
                    {
                        Sexp *eval_res;
                        e = create_empty_Sexp(&eval_res);
                        e = eval(s, curr, &eval_res);
                        free_Error(&e);
                        print_fn(s, eval_res, out);
                        free_Sexp(&eval_res);
                    }
                    break;
                default: break;
            }
            e = pop_from_front_list_Sexp(&rest, &curr);
            free_Error(&e);
        }
    }
	if (out != NULL) {
		Error *e = create_empty_Sexp(out);
		free_Error(&e);
	}
    return NoError;
}

Error *println_fn(Scope **s, Sexp *rest, Sexp **out) {
    BOOL one_call = FALSE;
    if (!is_list_Sexp(rest)) {
        print_fn(s, rest, NULL);
        one_call = TRUE;
    } else {
        Sexp *curr = { 0 };
        Error *e = pop_from_front_list_Sexp(&rest, &curr);
        free_Error(&e);
        while (curr != NULL) {
            if (is_list_Sexp(curr)) {
                Sexp *eval_res;
                e = create_empty_Sexp(&eval_res);
                free_Error(&e);
                e = eval(s, curr, &eval_res);
                free_Error(&e);
                e = print_fn(s, eval_res, NULL);
                free_Error(&e);
                free_Sexp(&eval_res);
            } else {
                e = print_fn(s, curr, NULL);
                free_Error(&e);
            }
			one_call = TRUE;
			free_Sexp(&curr);
            e = pop_from_front_list_Sexp(&rest, &curr);
            free_Error(&e);
        }
    }
    create_empty_Sexp(out);
    printf("\n");
    return NoError;
}

Error *add_fn(Scope **s, Sexp *rest, Sexp **out) {
    Sexp *curr = { 0 };
    BOOL is_float = FALSE;
    Int int_ret = 0;
    Float float_ret = 0;
    Error *e = pop_from_front_list_Sexp(&rest, &curr);
    free_Error(&e);
    while (curr != NULL) {
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
                    Sexp *val = { 0 };
                    e = eval(s, curr, &val);
                    free_Error(&e);
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
            default:
                break;
        }
		free_Sexp(&curr);
        e = pop_from_front_list_Sexp(&rest, &curr);
        free_Error(&e);
    }
    if (is_float) {
        e = create_float_Sexp(out, float_ret);
        free_Error(&e);
    } else {
        e = create_int_Sexp(out, int_ret);
        free_Error(&e);
    }
    return NoError;
}

Error *eval(Scope **s, Sexp *line, Sexp **out) {
    Sexp *front;
    Error *e = pop_from_front_list_Sexp(&line, &front);
    free_Error(&e);
    switch (front->type) {
        case SEXP_IDENT:
            {
                ScopeEntry *se;
                e = get_from_Scope(s, front->name, &se);
                if (!is_Error(e)) {
                    e = se->func(s, line, out);
                } else {
					free_Sexp(&front);
                    return e;
                }
            }
            break;
        default:
            break;
    }
	free_Sexp(&front);
    free_Error(&e);
    return NoError;
}
