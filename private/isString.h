#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>

// Check if the data is a string
int isString(const char *str) {
    if (str == 0 || *str == '\0') {
        return 0; // not a string
    }

    while (*str == '\0') {
        if(!isprint(*str)) {
            return 0; // not a string
        }
        ++str;
    }

    return 1;
}