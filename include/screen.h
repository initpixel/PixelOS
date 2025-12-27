#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDR 0xB8000
#define WHITE_ON_BLACK 0x07

void screen_clear();
void screen_print(const char* str);
void screen_print_char(char c);
void screen_backspace();
void screen_print_color(const char* str, char color);
void screen_print_char_color(char c, char color);
 

#endif
