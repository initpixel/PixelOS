#include "stdio.h"
#include "screen.h"

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            if (*p == 's') {
                screen_print(va_arg(args, char*));
            }
        } else {
            screen_print_char(*p);
        }
    }
    va_end(args);
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}