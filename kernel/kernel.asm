; ==================================================================
; PixelOS -- Pixel kernel
; Copyright (C) 2025 initpixel
; ==================================================================

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
    cli 
    mov ebx, [esp + 4]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23               ; User Stack Segment (SS)
    push 0x1000000 + 0x10000; User ESP

    pushfd                  ; EFLAGS
    pop eax
    or eax, 0x3200
    push eax

    push 0x1B               ; User Code Segment (CS)
    push ebx                ; User EIP (addr app)

    ; --- Debug ---
    mov byte [0xB8000], 'J'
    mov byte [0xB8001], 0x0F
    ; ---------------

    iretd                   ; Jump in RING 3


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
