bits 32

section .multiboot
    align 4
    dd 0x1BADB002              ; magic
    dd 0x00                    ; flags
    dd - (0x1BADB002 + 0x00)   ; checksum

global start
global inb
global outb
global inw
global jump_to_user_mode
global stack_space

extern kmain

section .text

start:
    cli
    mov esp, stack_space
    call kmain
    hlt

; --- Jump to User Mode ---
jump_to_user_mode:
cli ; 1. Disable interrupts before jumping
mov ebx, [esp + 4] ; 2. Get the entry address (application EIP)

; 3. Set up user data segments (0x23 = Index 4, RPL 3)
mov ax, 0x23
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax

; 4. Build the stack for the IRETD instruction
; The processor is waiting for: SS, ESP, EFLAGS, CS, EIP
push 0x23 ; User Stack Segment (SS)
push 0x90000 ; User Stack Pointer (ESP) - make sure the address is free!

pushfd ; Save current EFLAGS
pop eax
or eax, 0x200 ; Enable interrupt bit (IF) in future mode
push eax ; EFLAGS

push 0x1B ; User Code Segment (CS) (Index 3, RPL 3)
push ebx ; User Instruction Pointer (EIP)

iretd ; TRANSITION TO USER MODE

; --- I/O Functions ---
inb:
    mov edx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

inw:
    mov edx, [esp + 4]
    xor eax, eax
    in ax, dx
    ret

section .bss
resb 8192
stack_space:
