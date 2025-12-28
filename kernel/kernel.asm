bits 32
section .multiboot
    align 4
    dd 0x1BADB002          ; magic number
    dd 0x00                ; flags
    dd - (0x1BADB002 + 0x00) ; checksum

global start
global inb
global outb
global inw
global syscall_handler      ; Export for IDT (Interrupt Descriptor Table)
global jump_to_user_mode    ; Export for the kernel to switch to Ring 3
global stack_space

extern kmain                ; C kernel entry point
extern syscall_dispatcher   ; C function for syscall logic

; --- Switch to User Mode (Ring 3) ---
jump_to_user_mode:
    mov ebx, [esp + 4]    ; Get the application entry point (e.g., 0x400000)

    ; Set up the stack for IRETD to perform a privilege level switch
    ; The stack must contain: SS, ESP, EFLAGS, CS, EIP (in this order)
    
    push 0x23             ; User Data Segment Selector (Index 4, RPL 3)
    push 0x90000          ; User Stack Pointer (ESP)
    
    pushfd                ; Push current EFLAGS
    pop eax
    or eax, 0x200         ; Enable Interrupts (IF bit) in User Mode
    push eax
    
    push 0x1B             ; User Code Segment Selector (Index 3, RPL 3)
    push ebx              ; Application Entry Point (EIP)

    ; Set Data Segment registers to User Data Selector
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    iretd                 ; Execute the switch to Ring 3

start:
    cli                     ; Disable interrupts during setup
    mov esp, stack_space    ; Initialize the kernel stack pointer
    call kmain              ; Jump to the C kernel
    hlt                     ; Halt CPU if the kernel returns

; --- System Call Handler ---
syscall_handler:
    pushad                  ; Save all general-purpose registers
    push esp                ; Pass the pointer to the registers as an argument to C
    call syscall_dispatcher ; Execute the syscall logic in C
    add esp, 4              ; Clean up the stack
    popad                   ; Restore all general-purpose registers
    iretd                   ; Return from interrupt (restores CS, EIP, EFLAGS)

; --- I/O Functions (Port Access) ---
inb:
    mov edx, [esp + 4]      ; Port address
    xor eax, eax
    in al, dx               ; Read 1 byte
    ret

outb:
    mov edx, [esp + 4]      ; Port address
    mov al, [esp + 8]       ; Data byte
    out dx, al              ; Write 1 byte
    ret

inw:
    mov edx, [esp + 4]      ; Port address
    xor eax, eax
    in ax, dx               ; Read 2 bytes (word)
    ret

section .bss
resb 8192                   ; Reserve 8KB for the kernel stack
stack_space:
