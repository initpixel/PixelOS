# Настройки компилятора
CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -Iinclude -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld

# Папки
SRCDIR = lib
OBJDIR = objectFiles

# Автоматический поиск всех .c файлов в lib/ и добавление kernel.c
C_SOURCES = $(wildcard $(SRCDIR)/*.c) kernel.c
# Превращение путей .c в пути .o в папке objectFiles
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
# Добавляем ассемблерный файл
OBJ += $(OBJDIR)/kasm.o

# Главная цель
all: prepare kernel

# Создание папки для объектов
prepare:
	mkdir -p $(OBJDIR)

# Линковка
kernel: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Компиляция C файлов
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/kernel.o: kernel.c
	$(CC) $(CFLAGS) $< -o $@

# Компиляция ASM файла
$(OBJDIR)/kasm.o: kernel.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) kernel
