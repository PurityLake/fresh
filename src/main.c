#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fresh/fresh.h"

int main(int argc, char **argv) {
    printf("Fresh v%d.%d!\n", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);

    Error *e;

    char *buffer;
    size_t bufsize = 128;
    size_t characters;

    buffer = malloc(sizeof *buffer * bufsize);

    Scope *scope;
    e = create_Scope(&scope, 10, NULL);
    free_Error(&e);

    ScopeEntry *_print;
    ScopeEntry *_println;
    ScopeEntry *_add;

    e = create_function_ScopeEntry(&_print, "print", &print_fn);
    free_Error(&e);
    e = create_function_ScopeEntry(&_println, "println", &println_fn);
    free_Error(&e);
    e = create_function_ScopeEntry(&_add, "+", &add_fn);
    free_Error(&e);

    e = add_to_Scope(&scope, _print);
    free_Error(&e);
    e = add_to_Scope(&scope, _println);
    free_Error(&e);
    e = add_to_Scope(&scope, _add);
    free_Error(&e);

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
        if (!is_Error(e)) {
            free_Error(&e);
            Sexp *result;
            e = eval(&scope, total, &result);
            if (!is_empty_Sexp(result)) {
				Sexp *r;
                e = println_fn(&scope, result, &r);
                free_Error(&e);
                free_Sexp(&r);
            }
            free_Sexp(&total);
            free_Sexp(&result);
        } else {
            printf("%s\n", e->message);
            free_Error(&e);
        }
    }

    return 0;
}
