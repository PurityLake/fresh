#include <assert.h>
#include <stdio.h>

#include "fresh/fresh.h"

int main(int argc, char **argv) {
    printf("Hello Fresh v%d.%d!\n", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);

    Sexp *total = parse_line("(print \"Hello World!\")");
    Sexp *s1 = create_empty_sexp();
    Sexp *s2 = create_string_sexp("hello world from sexp!");
    Sexp *s3 = create_int_sexp(10);
    Sexp *s4 = create_float_sexp(42.0f);
    Sexp *s5 = create_empty_list_sexp();
    Sexp *s6 = create_list_sexp(5);
    Sexp *s7 = create_ident_sexp("ident");

    assert(is_empty_sexp(s1) == TRUE);
    assert(is_string_sexp(s2) == TRUE);
    assert(is_int_sexp(s3) == TRUE);
    assert(is_float_sexp(s4) == TRUE);
    assert(is_empty_list_sexp(s5) == TRUE);
    assert(is_list_sexp(s6) == TRUE);
    assert(is_ident_sexp(s7) == TRUE);

    free_sexp(&s1);
    free_sexp(&s2);
    free_sexp(&s3);
    free_sexp(&s4);
    free_sexp(&s5);
    free_sexp(&s6);
    free_sexp(&s7);
    free_sexp(&total);

    assert(s1 == NULL);
    assert(s2 == NULL);
    assert(s3 == NULL);
    assert(s4 == NULL);
    assert(s5 == NULL);
    assert(s6 == NULL);
    assert(s7 == NULL);

    return 0;
}