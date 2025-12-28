// include/gdt_idt.h
#ifndef GDT_IDT_H
#define GDT_IDT_H
#include <stdint.h>

void init_gdt();
void init_idt();
void init_tss(uint32_t ss0, uint32_t esp0);

#endif
