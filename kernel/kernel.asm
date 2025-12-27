bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

global start
global inb
global outb
global inw

extern kmain 		;this is defined in the c file

start:
	cli 				;block interrupts
	mov esp, stack_space
	call kmain
	hlt 				;halt the CPU

; --- Функции ввода-вывода для C ---

inb:
    mov edx, [esp + 4]    ; получить номер порта из стека
    xor eax, eax
    in al, dx             ; прочитать байт
    ret

outb:
    mov edx, [esp + 4]    ; получить номер порта
    mov al, [esp + 8]     ; получить данные
    out dx, al            ; записать байт
    ret

inw:
    mov edx, [esp + 4]    ; получить номер порта
    xor eax, eax
    in ax, dx             ; прочитать 16-битное слово
    ret

section .bss
resb 8192; 8KB for stack
stack_space:
