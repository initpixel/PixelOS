// apps/libpixel.h
#ifndef LIBPIXEL_H
#define LIBPIXEL_H

#include <stdint.h>

void syscall_print(const char* str);
void syscall_gets(char* buf);
void syscall_exec(const char* filename);
int strcmp(const char *s1, const char *s2);

#endif
