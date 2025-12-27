#ifndef STRING_H
#define STRING_H

#include <stdint.h>

int strlen(const char* s);
char* strcpy(char* dest, const char* src);
void itoa(uint32_t n, char* str);

#endif
