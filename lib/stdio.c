#include <stdarg.h>
#include <string.h>
#include <screen.h>

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
