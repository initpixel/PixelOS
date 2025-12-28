#include "gdt_idt.h"
#include <string.h>

// Link to the handler from interrupts.asm (or kernel.asm)
extern void syscall_handler();

// --- 1. Structures ---

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct tss_entry_struct {
    uint32_t prev_tss; uint32_t esp0; uint32_t ss0;
    uint32_t esp1; uint32_t ss1; uint32_t esp2; uint32_t ss2;
    uint32_t cr3; uint32_t eip; uint32_t eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs, ldt;
    uint16_t trap, iomap_base;
} __attribute__((packed));

// --- 2. Global Variables ---

struct gdt_entry gdt[6] __attribute__((aligned(16)));
struct gdt_ptr   gp;
struct idt_entry idt[256] __attribute__((aligned(16)));
struct idt_ptr   idtp;
struct tss_entry_struct tss_entry __attribute__((aligned(16)));

// --- 3. Internal Helpers ---

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// --- 4. Initialization Functions ---

void init_gdt() {
    gp.limit = (uint16_t)(sizeof(struct gdt_entry) * 6) - 1;
    gp.base  = (uint32_t)&gdt;

    memset(gdt, 0, sizeof(gdt));

    gdt_set_gate(0, 0, 0, 0, 0);                // Null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data

    __asm__ __volatile__(
        "lgdt %0\n"
        "pushl $0x08\n"
        "pushl $.next\n"
        "lretl\n"
        ".next:\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        : : "m"(gp) : "eax", "memory"
    );
}

void init_tss(uint32_t ss0, uint32_t esp0) {
    memset(&tss_entry, 0, sizeof(tss_entry));
    tss_entry.ss0  = ss0;
    tss_entry.esp0 = esp0;
    tss_entry.cs   = 0x08;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10;
    tss_entry.iomap_base = sizeof(tss_entry);

    gdt_set_gate(5, (uint32_t)&tss_entry, sizeof(tss_entry) - 1, 0x89, 0x40);
    __asm__ __volatile__("mov $0x28, %%ax; ltr %%ax" : : : "ax", "memory");
}

void init_idt() {
    idtp.limit = (uint16_t)(sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;
    memset(&idt, 0, sizeof(idt));

    // Fill with null-gates to prevent triple faults on unhandled interrupts
    for(int i = 0; i < 256; i++) {
        idt_set_gate(i, (uint32_t)syscall_handler, 0x08, 0x8E);
    }

    // Register syscall
    idt_set_gate(0x80, (uint32_t)syscall_handler, 0x08, 0xEE);
    __asm__ __volatile__("lidt %0" : : "m"(idtp));
}
