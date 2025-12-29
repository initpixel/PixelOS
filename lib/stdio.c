#include <stdarg.h>
#include <string.h>
#include <screen.h>

void khex_to_str(uint32_t n, char* str) {
    char* hex_chars = "0123456789ABCDEF";
    str[0] = '0';
    str[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        str[i + 2] = hex_chars[n & 0xF];
        n >>= 4;
    }
    str[10] = '\0';
}

void printf(char* format, ...) {
    va_list args;
    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            if (format[i] == 'd') {
                int n = va_arg(args, int);
                char str[12];
                itoa(n, str);
                screen_print(str);
            } else if (format[i] == 'c') {
                char c = (char)va_arg(args, int);
                screen_print_char(c);
            } else if (format[i] == 'x') {
                unsigned int n = va_arg(args, unsigned int);
                char str[11];
                khex_to_str(n, str);
                screen_print(str);
            } else if (format[i] == 's') {
                char* s = va_arg(args, char*);
                screen_print(s);
            }
        } else {
            screen_print_char(format[i]);
        }
    }
    va_end(args);
}
