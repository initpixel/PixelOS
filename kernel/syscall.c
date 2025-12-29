#include <stdint.h>
#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <screen.h>
#include <gdt_idt.h>
#include <fs/ext2.h>

void syscall_dispatcher(struct registers* regs) {
    printf("SYSCALL 1: EAX=%d, EBX=%x\n", regs->eax, regs->ebx);
}
