#include <stdint.h>
#include <io.h> 
#include <stdio.h>

extern void screen_print_char(char c);
extern void screen_backspace(void);

static int shift_pressed = 0;
static int caps_lock_active = 0;

static const unsigned char kbd_map_normal[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

static const unsigned char kbd_map_shift[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

char getchar() {
    uint8_t scancode;
    while(1) {
        while (!(inb(0x64) & 1)); // Wait for scan code
        scancode = inb(0x60);

        int is_break = scancode & 0x80;
        uint8_t key = scancode & 0x7F;

        if (is_break) {
            if (key == 0x2A || key == 0x36) {
                shift_pressed = 0;
            }
        } else {
            if (key == 0x2A || key == 0x36) {
                shift_pressed = 1;
                continue;
            }
            if (key == 0x3A) { // CAPS LOCK
                caps_lock_active = !caps_lock_active;
                continue;
            }

            if (key < sizeof(kbd_map_normal)) {
                char c_normal = kbd_map_normal[key];
                char c_shift = kbd_map_shift[key];
                
                int is_alpha = (c_normal >= 'a' && c_normal <= 'z');
                
                char final_char;
                if (is_alpha) {
                    final_char = (caps_lock_active ^ shift_pressed) ? c_shift : c_normal;
                } else {
                    final_char = shift_pressed ? c_shift : c_normal;
                }

                if (final_char != 0) return final_char;
            }
        }
    }
}

void gets(char* buffer) {
    int i = 0;
    while (1) {
        char c = getchar();
        
        if (c == '\n') {
            buffer[i] = '\0';
            screen_print_char('\n');
            break;
        } 
        else if (c == '\b') { // Backspace
            if (i > 0) {
                i--;
                screen_backspace(); 
            }
        } 
        else if (c > 0) { 
            if (i < 255) { // Protect buffer size [256]
                buffer[i++] = c;
                screen_print_char(c); 
            }
        }
    }
}
