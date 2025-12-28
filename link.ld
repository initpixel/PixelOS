OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
/* The kernel is loaded at address 1MB */
. = 0x100000;

/* Code section */
.text BLOCK(4K) : ALIGN(4K)
{
*(.multiboot)
*(.text)
}

/* Data section (read-only) */
.rodata BLOCK(4K) : ALIGN(4K)
{
*(.rodata)
}

/* Data section (read and write) */
.data BLOCK(4K) : ALIGN(4K)
{
*(.data)
}

/* BSS section (your GDT, IDT, and stack are here) */
.bss BLOCK(4K) : ALIGN(4K)
{
*(COMMON)
*(.bss)
}

/* Specify the end of the kernel for debugging */
_kernel_end = .;
}