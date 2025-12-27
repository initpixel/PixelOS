#include "screen.h"
#include "io.h"

static int cursor = 0;
static char* vidptr = (char*)VIDEO_ADDR;

void update_hardware_cursor() {
    uint16_t position = cursor / 2;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

void screen_scroll() {
    for (int i = 0; i < 80 * 24 * 2; i++) {
        vidptr[i] = vidptr[i + 80 * 2];
    }

    for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
        vidptr[i] = ' ';
        vidptr[i + 1] = WHITE_ON_BLACK;
    }

    cursor = 80 * 24 * 2;
}


void screen_clear() {
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vidptr[i] = ' ';
        vidptr[i+1] = WHITE_ON_BLACK;
    }
    cursor = 0;
    update_hardware_cursor();
}

void screen_print_char(char c) {
    
    if (cursor >= 80 * 25 * 2) {
        screen_scroll();
    }

    if (c == '\n') {
        cursor = cursor + (160 - cursor % 160);
    } else {
        vidptr[cursor++] = c;
        vidptr[cursor++] = WHITE_ON_BLACK;
    }
    update_hardware_cursor();
}

void screen_backspace() {
    if (cursor > 0) {
        cursor -= 2;
        vidptr[cursor] = ' ';
        vidptr[cursor+1] = WHITE_ON_BLACK;
        update_hardware_cursor();
    }
}

void screen_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        screen_print_char(str[i]);
    }
}

void screen_print_char_color(char c, char color) {
    char* vidptr = (char*)0xb8000;
    if (c == '\n') {
        cursor = cursor + (160 - cursor % 160);
    } else if (cursor < 80 * 25 * 2) {
        vidptr[cursor++] = c;
        vidptr[cursor++] = color;
    }
    update_hardware_cursor();
}

void screen_print_color(const char* str, char color) {
    for (int i = 0; str[i] != '\0'; i++) {
        screen_print_char_color(str[i], color);
    }
}
