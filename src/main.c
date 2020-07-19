#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fresh/fresh.h"

int main(int argc, char **argv) {
    printf("Hello Fresh v%d.%d!\n", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);

    //Sexp *total = parse_line("(println \"Hello World!\" 1 2.3 ident)");
    //Sexp *out = eval(total);
    //("(println 1 2 (+ 3 4 5 6) 4.2341 \"Hello World!\" ident?)\n");
    //Sexp *total = parse_line("(println 1 2 (+ 3 4 5 6) 4.2341 \"Hello World!\" ident?)");
    //Sexp *result = eval(total);
    //printf("(+ 1 2 (+ 3 4 (+ 5 6)))\n");
    //total = parse_line("(+ 1 2 (+ 3 4 (+ 5 6)))");
    //result = eval(total);
    //printf("%ld\n", result->i);
    Error *e;

    char *buffer;
    size_t bufsize = 128;
    size_t characters;

    buffer = (char *)malloc(sizeof(char) * bufsize);

    while (TRUE) {
        memset(buffer, '\0', bufsize);
        printf("> ");
        characters = getline(&buffer, &bufsize, stdin);
        buffer[characters-1] = '\0';
        if (strcmp(buffer, "quit") == 0) {
            break;
        }
        Sexp *total = { 0 };
        e = parse_line(&total, buffer);
        Sexp *result = { 0 };
        e = eval(total, &result);
        if (!is_empty_Sexp(result)) {
            Sexp *r = { 0 };
            e = println_fn(result, &r);
            free_Error(&e);
            free_Sexp(&r);
        }
        free_Sexp(&total);
        free_Sexp(&result);
    }

    return 0;
}