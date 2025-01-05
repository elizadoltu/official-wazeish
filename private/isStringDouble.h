#ifndef IS_STRING_DOUBLE_H
#define IS_STRING_DOUBLE_H

#include <iostream>
#include <sstream>
#include <string.h>

bool isDouble(std::string &text) {
    try {
        std::stod(text);
        return true;
    } catch (std::invalid_argument&) {
        return false; 
    } catch (std::out_of_range&) {
        return false;
    }
}

#endif // IS_STRING_DOUBLE_H