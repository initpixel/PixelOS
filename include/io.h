#ifndef IO_H
#define IO_H

#include <stdint.h> // Типы должны быть и здесь

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

void pic_remap(int offset1, int offset2);
void reboot();

#endif
