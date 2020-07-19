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
        if (!is_Error(e)) {
            free_Error(&e);
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
        } else {
            printf("%s\n", e->message);
            free_Error(&e);
        }
    }

    return 0;
}