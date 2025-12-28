#include <syscall.h>
#include <stdio.h>    // for printf
#include <fs/ext2.h>  // for ext2_ls
#include <screen.h>

void syscall_dispatcher(struct registers* regs) {
    
    uint32_t syscall_num = regs->eax;
    
    switch (syscall_num) {
        case 1: 
            if (regs->ebx != 0) {
                printf("%s", (char*)regs->ebx); 
            }
            break;

        case 2: 
            ext2_ls();
            break;

        case 3: 
            screen_clear();
            break;

        default:
            printf("Unknown syscall: %d (HEX: 0x%x)\n", regs->eax, regs->eax);
            break;
    }
}
