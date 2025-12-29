// kernel/shell.c
#include <stdio.h>
#include <string.h>
#include <fs/ext2.h>
#include <debug/debug.h>
#include <screen.h>

void shell_process(char* input) {
    if (strlen(input) == 0) {
        return;
    }

    if (strcmp(input, "help") == 0) {
        printf("PixelOS Shell. Available commands:\n");
        printf("help  - show this message\n");
        printf("ls    - list files on ext2 disk\n");
        printf("run   - run hello.bin app\n");
        printf("clear - clear the screen\n");
    } 

    else if (strcmp(input, "ls") == 0) {
        ext2_ls();
    } 

    else if (strcmp(input, "kernal") == 0) {
        screen_clear();
        printf("==================================================================\n");
        printf("    PixelOS -- Pixel kernel\n");
        printf("    Copyright (C) 2025 initpixel\n");
        printf("             v0.4               \n");
        printf("==================================================================\n");
    } 

    else if (strcmp(input, "view_ring") == 0) {
        asm volatile (
            "movl $2, %%eax\n"
            "int $0x80"
            : : : "eax"
        );
    }


    else if (strcmp(input, "run") == 0) {
        run_app("hello.bin");
    }

    else if (strcmp(input, "clear") == 0) {
        screen_clear();
    } 

    else {
        printf("Unknown command: ");
        printf(input);
        printf("\nType 'help' for list of commands.\n");
    }
}
