#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// Инициализация клавиатуры (если понадобится в будущем)
void keyboard_init();

// Читает символ из буфера (опрос порта)
// Возвращает ASCII символ или 0, если клавиша не нажата
char get_char();

// Функция для обработки нажатия (вызывается в цикле kmain)
// В неё можно добавить логику обработки Shift, Ctrl и т.д.
void handle_keyboard();

#endif
