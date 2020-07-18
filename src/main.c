#include <assert.h>
#include <stdio.h>

#include "fresh/fresh.h"

int main(int argc, char **argv) {
    printf("Hello Fresh v%d.%d!\n", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);
    struct sexp *total = parse_line("(1 1.2 (\"hello\" + 3.2 1))");
    struct sexp *s1 = create_empty_sexp();
    struct sexp *s2 = create_string_sexp("hello world from sexp!");
    struct sexp *s3 = create_int_sexp(10);
    struct sexp *s4 = create_float_sexp(42.0f);
    struct sexp *s5 = create_empty_list_sexp();
    struct sexp *s6 = create_list_sexp(5);
    struct sexp *s7 = create_ident_sexp("ident");

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

    assert(s1 == NULL);
    assert(s2 == NULL);
    assert(s3 == NULL);
    assert(s4 == NULL);
    assert(s5 == NULL);
    assert(s6 == NULL);
    assert(s7 == NULL);
    return 0;
}