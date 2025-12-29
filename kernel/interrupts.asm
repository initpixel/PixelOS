bits 32
global syscall_handler
global dummy_handler
extern syscall_dispatcher

dummy_handler:
    iretd

syscall_handler:
    mov byte [0xB8000], '!' ; Выведет '!' в левом верхнем углу экрана
    mov byte [0xB8001], 0x0F

    pushad    ; 1. Пушит EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    push fs   ; 2.
    push gs   ; 3.
    push es   ; 4.
    push ds   ; 5. Последний PUSH (самый низкий адрес)

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    push esp  ; Передаем указатель на DS
    call syscall_dispatcher
    add esp, 4

    pop ds    ; Первый POP
    pop es
    pop gs
    pop fs
    popad
    iretd
