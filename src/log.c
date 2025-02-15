#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void error(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
    exit(EXIT_FAILURE);
}
