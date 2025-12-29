void print(const char* str) {
    __asm__ __volatile__ (
        "int $0x80" 
        : 
        : "a"(1), "b"(str)
    );
}

void _start() {
    print("Hello from User Space!\n");

    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(2)
    );
}
