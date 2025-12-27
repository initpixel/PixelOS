#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

void printf(const char* format, ...);
int strcmp(const char* s1, const char* s2);
void gets(char* buffer);
char getchar(void);

#endif
