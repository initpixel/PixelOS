CC = gcc
AS = nasm
LD = ld

# Output binary name
TARGET = pixelos.bin

# Compilation flags
CFLAGS = -m32 -ffreestanding -Iinclude -c -Wall
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld

# Directories
LIBDIR = lib
KERNELDIR = kernel
OBJDIR = objectFiles

# --- FILE SEARCH LOGIC ---

# Search for all .c files in lib/, kernel/ and all subdirectories (e.g., kernel/fs/)
C_SOURCES = $(wildcard $(LIBDIR)/*.c) \
            $(wildcard $(KERNELDIR)/*.c) \
            $(wildcard $(KERNELDIR)/**/*.c)

# Main assembly source file
ASM_SOURCE = $(KERNELDIR)/kernel.asm

# Convert .c source paths to .o object files inside OBJDIR
# $(notdir) is used to prevent path conflicts for object files
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
OBJ += $(OBJDIR)/kasm.o

# --- BUILD RULES ---

all: prepare $(TARGET)

prepare:
	mkdir -p $(OBJDIR)

# Link the final binary
$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile files from lib/
$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile files from kernel/
$(OBJDIR)/%.o: $(KERNELDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile files from nested directories (e.g., kernel/fs/)
$(OBJDIR)/%.o: $(KERNELDIR)/*/%.c
	$(CC) $(CFLAGS) $< -o $@

# Compile assembly source
$(OBJDIR)/kasm.o: $(ASM_SOURCE)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
