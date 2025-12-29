// apps/shell.c

void __attribute__((section(".text._start"))) _start() {
    asm volatile (
        "movl $1, %%eax\n"
        "int $0x80"
        : : : "eax"
    );
    while(1); 
}
