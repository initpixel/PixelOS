#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>

struct registers {
    uint32_t ds, es, gs, fs; 
    uint32_t edi, esi, ebp, esp; 
    uint32_t ebx, edx, ecx, eax; 
    uint32_t eip, cs, eflags, useresp, ss; 
} __attribute__((packed));



void syscall_dispatcher(struct registers* regs);

#endif
