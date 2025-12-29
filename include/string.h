#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void itoa(int n, char* s);
void* memcpy(void* dest, const void* src, size_t n);
size_t strlen(const char* s);
int strncmp(const char* s1, const char* s2, size_t n);
int strcmp(const char* s1, const char* s2);
void* memset(void* s, int c, size_t n);

#endif
