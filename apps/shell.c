#include <stdint.h>

void print(char* msg) {
    asm volatile ("mov $1, %%eax; mov %0, %%ebx; int $0x80" : : "r"(msg) : "eax", "ebx");
}

void run_program(char* name) {
    asm volatile ("mov $4, %%eax; mov %0, %%ebx; int $0x80" : : "r"(name) : "eax", "ebx");
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void main() {
    print("PixelOS Shell Starting...\n");
    run_program("hello.bin");
    while(1);
}