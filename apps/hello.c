void print(char* msg) {
    asm volatile ("mov $1, %%eax; mov %0, %%ebx; int $0x80" : : "r"(msg) : "eax", "ebx");
}

void main() {
    print("Hello! I am a real app on PixelOS disk!\n");
    while(1); 
}