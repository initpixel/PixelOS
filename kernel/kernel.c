// ==================================================================
// PixelOS -- Pixel kernel
// Copyright (C) 2025 initpixel
// ==================================================================

#include <stdio.h>
#include <string.h>
#include <drivers/disk.h>
#include <fs/ext2.h>
#include <screen.h>
#include <gdt_idt.h>

extern void init_gdt();
extern void init_idt();
extern void init_tss(uint32_t ss0, uint32_t esp0);
extern void shell_process(char* input);
extern void jump_to_user_mode(uint32_t entry_point);
extern uint32_t stack_space;
static uint8_t kernel_stack[8192]; 

void kmain() {
    screen_clear();
    __asm__ __volatile__("cli");

    init_gdt();
    init_tss(0x10, (uint32_t)kernel_stack + 8192);
    init_idt();
    
    printf("Kernel initialized. Launching shell...\n");

    run_app("shell.bin"); 

    printf("KERNEL ERROR: We should be in Ring 3, but we are still here!\n");
    
    while(1) { __asm__ __volatile__("hlt"); };
    
}
