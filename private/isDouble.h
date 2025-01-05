#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Check if it is a double 
int isDouble(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // not an integer
    }

    // Check each character to be a dot or a digit
    int dotCount = 0;
    while(*str != '\0') {
        if (!isdigit(*str)) {
            if (*str == '.' && dotCount == 0)
                ++dotCount;
            else {
                return 0;
            }
        }
        ++str;
    }

    return 1;
}