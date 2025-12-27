bits 32
section .text
        ; Multiboot specification header
        align 4
        dd 0x1BADB002              ; magic number
        dd 0x00                    ; flags
        dd - (0x1BADB002 + 0x00)   ; checksum

global start
global inb
global outb
global inw
global syscall_handler      ; Export the handler for IDT registration

extern kmain                ; Defined in C kernel code
extern syscall_dispatcher   ; C function to handle specific syscall logic

start:
    cli                     ; Clear interrupts
    mov esp, stack_space    ; Setup kernel stack pointer
    call kmain              ; Jump to C code
    hlt                     ; Halt CPU if kmain returns

; --- System Calls ---
syscall_handler:
    pushad                  ; Save all general-purpose registers
    push esp                ; Push pointer to registers as an argument for C
    call syscall_dispatcher ; Call the C dispatcher
    add esp, 4              ; Clean up stack after call
    popad                   ; Restore all general-purpose registers
    iretd                   ; Interrupt return (restore EIP, CS, EFLAGS)

; --- I/O Functions ---
inb:
    mov edx, [esp + 4]      ; Get port number from stack
    xor eax, eax
    in al, dx               ; Read byte from port
    ret

outb:
    mov edx, [esp + 4]      ; Get port number
    mov al, [esp + 8]       ; Get data byte
    out dx, al              ; Write byte to port
    ret

inw:
    mov edx, [esp + 4]      ; Get port number
    xor eax, eax
    in ax, dx               ; Read word (16-bit) from port
    ret

section .bss
resb 8192                   ; Reserve 8KB for the stack
stack_space:
