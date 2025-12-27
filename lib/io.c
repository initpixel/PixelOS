#include <stdio.h>
#include "io.h"

// Перенастройка контроллера прерываний (PIC)
// По умолчанию прерывания оборудования в x86 конфликтуют с исключениями процессора.
// Эта функция говорит PIC слать сигналы на свободные векторы (например, 0x20 и 0x28).
void pic_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(0x21); // Сохраняем маски
    a2 = inb(0xA1);

    outb(0x20, 0x11); // Начинаем инициализацию
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    
    outb(0x21, offset1); // Устанавливаем смещение
    io_wait();
    outb(0xA1, offset2);
    io_wait();

    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, a1); // Восстанавливаем маски
    outb(0xA1, a2);
}

// Пример функции "мягкой" перезагрузки через контроллер клавиатуры
void reboot() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}
