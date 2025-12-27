#include <stdio.h>
#include <string.h>
#include <drivers/disk.h>
#include <fs/ext2.h>
#include <screen.h>

extern void init_gdt();
extern void init_idt();
extern void shell_process(char* input);

void kmain() {
    screen_clear();
    char input_buffer[256];
    
    while(1) {
        screen_print("PixelOS> ");
        gets(input_buffer);
        shell_process(input_buffer);
    }
}
