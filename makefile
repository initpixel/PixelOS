CC = gcc
AS = nasm
LD = ld

TARGET = pixelos.bin

CFLAGS = -m32 -ffreestanding -Iinclude -c -Wall -fno-pic -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS_KERN = -m elf_i386 -T link.ld
LDFLAGS_APPS = -m elf_i386 -T apps/app.ld

LIBDIR = lib
KERNELDIR = kernel
APPSDIR = apps
OBJDIR = objectFiles

C_SOURCES = $(wildcard $(LIBDIR)/*.c) \
            $(wildcard $(KERNELDIR)/*.c) \
            $(wildcard $(KERNELDIR)/**/*.c)

ASM_SOURCE = $(KERNELDIR)/kernel.asm

OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
OBJ += $(OBJDIR)/kasm.o

APP_SOURCES = $(wildcard $(APPSDIR)/*.c)
APP_BINS = $(patsubst $(APPSDIR)/%.c, %.bin, $(APP_SOURCES))

all: prepare $(TARGET) $(APP_BINS)

prepare:
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS_KERN) -o $@ $^

%.bin: $(APPSDIR)/%.c
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$*.app.o
	$(LD) $(LDFLAGS_APPS) -o $@ $(OBJDIR)/$*.app.o

$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(KERNELDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(KERNELDIR)/*/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/kasm.o: $(ASM_SOURCE)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) *.bin
