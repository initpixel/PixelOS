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

void kmain() {
    screen_clear();

    init_gdt();
    
    init_tss(0x10, 0x90000); 
    
    init_idt();
    
    char input_buffer[256];
    
    while(1) {
        screen_print("Shell> ");
        
        memset(input_buffer, 0, 256);
        
        gets(input_buffer);
        
        if (strcmp(input_buffer, "run") == 0) {
            run_app("hello.bin");
        } else {
            shell_process(input_buffer);
        }
    }
}
