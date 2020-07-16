#include <stdio.h>

#include "fresh.h"
#include "fresh_config.h"

int main(int argc, char **argv) {
    printf("Hello Fresh v%d.%d!", FRESH_VERSION_MAJOR, FRESH_VERSION_MINOR);
    return 0;
}