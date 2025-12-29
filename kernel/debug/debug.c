#include <debug/debug.h>
#include <stdio.h>
#include <screen.h>

void debug_dump_regs(struct registers* regs) {
    printf("\n--- REGISTER DUMP ---\n");
    printf("EAX: %x EBX: %x ECX: %x EDX: %x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
    printf("ESI: %x EDI: %x EBP: %x ESP: %x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
    printf("EIP: %x CS:  %x EFL: %x\n",          regs->eip, regs->cs,  regs->eflags);
    printf("---------------------\n");
}

void kernel_panic(const char* reason, struct registers* regs) {
    __asm__ __volatile__("cli");

    screen_clear();
    printf("KERNEL PANIC: %s\n", reason);
    
    if (regs) {
        debug_dump_regs(regs);
    }

    printf("\nSYSTEM HALTED.");
    while(1) { __asm__ __volatile__("hlt"); }
}

void view_ring(struct registers* regs) {
    uint8_t cpl = regs->cs & 3;
    
    if (cpl == 0) {
        printf("DEBUG: Called from Ring 0 (Kernel)\n");
    } else if (cpl == 3) {
        printf("DEBUG: Called from Ring 3 (User Mode)\n");
    }
    
    printf("Full CS: %x, EAX: %d\n", regs->cs, regs->eax);
}
