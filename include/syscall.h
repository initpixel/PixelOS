#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

struct registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
} __attribute__((packed));

void syscall_dispatcher(struct registers* regs);

#endif
