#include "screen.h"
#include "stdio.h"
#include "ascii_art.h"
#include "keyboard.h"


#define MAX_CMD_LEN 128
char cmd_buffer[MAX_CMD_LEN];
int cmd_idx = 0;

void execute_command(char* cmd) {
    printf("\n");
    if (strcmp(cmd, "system") == 0) {
        screen_clear();
        print_logo();
        printf("Copyright 2025 initpixel\n");
        printf("github: https://github.com/initpixel\n");
    } else if (strcmp(cmd, "clear") == 0) {
        screen_clear();
    } else if (strcmp(cmd, "ver") == 0) {
        printf("initpixel");
    } else if (cmd[0] == '\0') {
    } else {
        printf("Unknown: %s\n", cmd);
    }
    printf("system> ");
}

void on_key_pressed(char c) {
    if (c == '\n') { // Enter
        cmd_buffer[cmd_idx] = '\0';
        execute_command(cmd_buffer);
        cmd_idx = 0;
    } else if (c == '\b') { // Backspace
        if (cmd_idx > 0) {
            cmd_idx--;
            screen_backspace();
        }
    } else if (c >= 32 && c <= 126) { // Печатаемые символы
        if (cmd_idx < MAX_CMD_LEN - 1) {
            cmd_buffer[cmd_idx++] = c;
            screen_print_char(c);
        }
    }
}

void kmain(void) {
    screen_clear();
    print_logo();
    printf("Copyright 2025 initpixel\n");
    printf("github: https://github.com/initpixel\n");
    printf("system> ");

    while(1) {
        char c = get_char(); 
        
        if (c != 0) {
            on_key_pressed(c);
        }
        
    }
}