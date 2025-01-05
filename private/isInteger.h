#ifndef IS_INTEGER_H
#define IS_INTEGER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Check if it is an integer
int isInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // not an integer
    }

    // Check each character to be a digit
    while (*str != '\0') {
        if(!isdigit(*str)) {
            return 0; // Not an integer
        }
        ++str;
    }

    return 1;
}

#endif 