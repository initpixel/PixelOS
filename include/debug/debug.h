#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <syscall.h>

void kernel_panic(const char* reason, struct registers* regs);

void debug_dump_regs(struct registers* regs);

void view_ring(struct registers* regs);

#endif
