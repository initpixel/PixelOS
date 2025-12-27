#include <syscall.h>
#include <stdio.h>    // for printf
#include <fs/ext2.h>  // for ext2_ls
#include <screen.h>

void syscall_dispatcher(struct registers* regs) {
    switch (regs->eax) {
        case 1: 
            printf((char*)regs->ebx); 
            break;

        case 2: 
            ext2_ls();
            break;

        case 3: 
            screen_clear();
            break;

        default:
            printf("Unknown syscall: %d\n", regs->eax);
            break;
    }
}
