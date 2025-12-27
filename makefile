# Compiler and Linker definitions
CC = gcc
AS = nasm
LD = ld

# Final kernel binary name
TARGET = pixelos.bin

# Compilation and Linking flags
CFLAGS = -m32 -ffreestanding -Iinclude -c -Wall -fno-pic -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS_KERN = -m elf_i386 -T link.ld

# Apps linking flags (Apps are linked to a different base address)
LDFLAGS_APPS = -m elf_i386 -T apps/app.ld

# Directory structure
LIBDIR = lib
KERNELDIR = kernel
APPSDIR = apps
OBJDIR = objectFiles

# --- KERNEL SOURCE DISCOVERY ---

# Find all C files in lib/, kernel/ and subfolders (like kernel/fs/ or kernel/drivers/)
C_SOURCES = $(wildcard $(LIBDIR)/*.c) \
            $(wildcard $(KERNELDIR)/*.c) \
            $(wildcard $(KERNELDIR)/**/*.c)

# Main assembly entry point
ASM_SOURCE = $(KERNELDIR)/kernel.asm

# Generate object file list for the kernel
# $(notdir) prevents directory prefix in object names to keep OBJDIR flat
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
OBJ += $(OBJDIR)/kasm.o

# --- APPLICATION SOURCE DISCOVERY ---

# Every .c file in apps/ will be compiled into a separate .bin file
APP_SOURCES = $(wildcard $(APPSDIR)/*.c)
APP_BINS = $(patsubst $(APPSDIR)/%.c, %.bin, $(APP_SOURCES))

# --- BUILD TARGETS ---

# Default target: build the kernel and all discovered apps
all: prepare $(TARGET) $(APP_BINS)

# Create necessary directories
prepare:
	mkdir -p $(OBJDIR)

# Link the final kernel binary
$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS_KERN) -o $@ $^

# Pattern rule for building applications
# Each app is compiled to a temporary .o and then linked into a flat .bin
%.bin: $(APPSDIR)/%.c
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$*.app.o
	$(LD) $(LDFLAGS_APPS) -o $@ $(OBJDIR)/$*.app.o

# Rules for compiling kernel-related object files from different source directories
$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(KERNELDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(KERNELDIR)/*/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile the main assembly file
$(OBJDIR)/kasm.o: $(ASM_SOURCE)
	$(AS) $(ASFLAGS) $< -o $@

# Remove build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET) *.bin
