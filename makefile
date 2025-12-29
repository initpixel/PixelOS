# ==================================================================
# PixelOS Build System (2025)
# ==================================================================

# Compilers and Linker
CC = gcc
AS = nasm
LD = ld

# Output Kernel Binary
TARGET = pixelos.bin

# Compilation Flags
# -m32: Compile for 32-bit x86 architecture
# -ffreestanding: Required for OS development (no standard library)
# -fno-stack-protector: Disable stack smashing protection (requires OS support)
# -fno-pic: Disable position-independent code for simplicity
CFLAGS = -m32 -ffreestanding -Iinclude -Ikernel -c -Wall -fno-pic -fno-stack-protector
ASFLAGS = -f elf32

# Linker Flags
# -T: Use custom linker scripts
# -e _start: Explicitly set entry point for user applications
LDFLAGS_KERN = -m elf_i386 -T link.ld
LDFLAGS_APPS = -m elf_i386 -T apps/app.ld -e _start

# Directories
LIBDIR = lib
KERNELDIR = kernel
APPSDIR = apps
OBJDIR = objectFiles

# 1. Kernel Source Discovery
C_SOURCES = $(wildcard $(LIBDIR)/*.c) \
            $(wildcard $(KERNELDIR)/*.c) \
            $(wildcard $(KERNELDIR)/**/*.c)

ASM_SOURCES = $(wildcard $(KERNELDIR)/*.asm)

# VPATH for automatic source lookup
vpath %.c $(LIBDIR) $(KERNELDIR) $(KERNELDIR)/debug $(KERNELDIR)/fs $(KERNELDIR)/drivers
vpath %.asm $(KERNELDIR)

# Transform source lists into object file lists
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
OBJ += $(patsubst %.asm, $(OBJDIR)/%_asm.o, $(notdir $(ASM_SOURCES)))

# 2. Application Discovery
# We exclude libpixel.c from direct binary generation
APP_SOURCES = $(filter-out $(APPSDIR)/libpixel.c, $(wildcard $(APPSDIR)/*.c))
APP_BINS = $(patsubst $(APPSDIR)/%.c, %.bin, $(APP_SOURCES))

# User Runtime Library (The bridge between Apps and Kernel)
LIBPIXEL_OBJ = $(OBJDIR)/libpixel.app.o

# --- Build Rules ---

all: prepare $(TARGET) $(APP_BINS)

# Create the object directory if it doesn't exist
prepare:
	@mkdir -p $(OBJDIR)

# Link the kernel binary
$(TARGET): $(OBJ)
	@echo "Linking Kernel: $@"
	$(LD) $(LDFLAGS_KERN) -o $@ $^

# Rule for compiling Kernel C files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Rule for assembling Kernel ASM files
$(OBJDIR)/%_asm.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

# --- User Mode Applications Rules ---

# Compile the User Runtime Library (libpixel)
$(LIBPIXEL_OBJ): $(APPSDIR)/libpixel.c
	@echo "Compiling User Library: $<"
	$(CC) $(CFLAGS) $< -o $@

# Rule for building user applications (.bin)
# Links the application code with libpixel to resolve syscalls
%.bin: $(APPSDIR)/%.c $(LIBPIXEL_OBJ)
	@echo "Building Application: $@"
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$*.app.o
	$(LD) $(LDFLAGS_APPS) -o $@ $(OBJDIR)/$*.app.o $(LIBPIXEL_OBJ)

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET) *.bin

.PHONY: all prepare clean
