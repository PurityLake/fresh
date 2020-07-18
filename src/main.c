#include <assert.h>
#include <stdio.h>

#include "fresh/fresh.h"

int main(int argc, char **argv) {
    printf("Hello Fresh v%d.%d!\n", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);

    Sexp *total = parse_line("(print \"Hello World!\")");
    Sexp *s1 = create_empty_Sexp();
    Sexp *s2 = create_string_Sexp("hello world from sexp!");
    Sexp *s3 = create_int_Sexp(10);
    Sexp *s4 = create_float_Sexp(42.0f);
    Sexp *s5 = create_empty_list_Sexp();
    Sexp *s6 = create_list_Sexp(5);
    Sexp *s7 = create_ident_Sexp("ident");

    assert(is_empty_Sexp(s1) == TRUE);
    assert(is_string_Sexp(s2) == TRUE);
    assert(is_int_Sexp(s3) == TRUE);
    assert(is_float_Sexp(s4) == TRUE);
    assert(is_empty_list_Sexp(s5) == TRUE);
    assert(is_list_Sexp(s6) == TRUE);
    assert(is_ident_Sexp(s7) == TRUE);

    free_Sexp(&s1);
    free_Sexp(&s2);
    free_Sexp(&s3);
    free_Sexp(&s4);
    free_Sexp(&s5);
    free_Sexp(&s6);
    free_Sexp(&s7);
    free_Sexp(&total);

    assert(s1 == NULL);
    assert(s2 == NULL);
    assert(s3 == NULL);
    assert(s4 == NULL);
    assert(s5 == NULL);
    assert(s6 == NULL);
    assert(s7 == NULL);

    return 0;
}