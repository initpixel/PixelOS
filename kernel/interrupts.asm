bits 32

global syscall_handler
extern syscall_dispatcher

section .text

syscall_handler:
    ; 1. Processor pushed: SS, ESP, EFLAGS, CS, EIP
    ; 2. Push general purpose registers (EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX)
    pushad    
    
    ; 3. Push segments (Matches 'ds' and 'es' in your C struct)
    push ds
    push es

    ; 4. Load Kernel Data Segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 5. Pass ESP (pointer to the whole stack frame) to C
    push esp
    call syscall_dispatcher
    add esp, 4      ; Clean up the argument from stack

    ; 6. Restore everything
    pop es
    pop ds
    popad
    iretd
