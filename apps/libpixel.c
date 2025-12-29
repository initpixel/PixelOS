#include "libpixel.h"

void syscall_print(const char* str) {
    asm volatile (
        "movl $1, %%eax\n"
        "movl %0, %%ebx\n"
        "int $0x80"
        : : "g"(str) : "eax", "ebx"
    );
}

void syscall_gets(char* buf) {
    asm volatile (
        "movl $3, %%eax\n"
        "movl %0, %%ebx\n"
        "int $0x80"
        : : "r"(buf) : "eax", "ebx"
    );
}

void syscall_exec(const char* filename) {
    asm volatile (
        "movl $11, %%eax\n"
        "movl %0, %%ebx\n"
        "int $0x80"
        : : "r"(filename) : "eax", "ebx"
    );
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

