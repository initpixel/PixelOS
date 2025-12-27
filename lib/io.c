#include <stdio.h>
#include "io.h"

void pic_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(0x21); // Keep the masks
    a2 = inb(0xA1);

    outb(0x20, 0x11); // Start init
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    
    outb(0x21, offset1); 
    io_wait();
    outb(0xA1, offset2);
    io_wait();

    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, a1); 
    outb(0xA1, a2);
}

void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}
