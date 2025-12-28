CC = gcc
AS = nasm
LD = ld

TARGET = pixelos.bin

# Флаги компиляции
CFLAGS = -m32 -ffreestanding -Iinclude -c -Wall -fno-pic -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS_KERN = -m elf_i386 -T link.ld
LDFLAGS_APPS = -m elf_i386 -T apps/app.ld

# Папки
LIBDIR = lib
KERNELDIR = kernel
BINDIR = bin
APPSDIR = apps
OBJDIR = objectFiles

# 1. Поиск всех исходников C
C_SOURCES = $(wildcard $(LIBDIR)/*.c) \
            $(wildcard $(KERNELDIR)/*.c) \
            $(wildcard $(KERNELDIR)/**/*.c)

# 2. Поиск всех исходников ASM в папке kernel (kernel.asm, interrupts.asm и т.д.)
ASM_SOURCES = $(wildcard $(KERNELDIR)/*.asm)

# 3. Формирование списка объектных файлов (.o) в папке objectFiles
# Для C файлов
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(C_SOURCES)))
# Для ASM файлов (добавляем суффикс _asm, чтобы не было конфликтов имен)
OBJ += $(patsubst %.asm, $(OBJDIR)/%_asm.o, $(notdir $(ASM_SOURCES)))

# Поиск исходников приложений
APP_SOURCES = $(wildcard $(APPSDIR)/*.c)
APP_BINS = $(patsubst $(APPSDIR)/%.c, %.bin, $(APP_SOURCES))

# --- Правила сборки ---

all: prepare $(TARGET) $(APP_BINS)

prepare:
	mkdir -p $(OBJDIR)

# Линковка ядра
$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS_KERN) -o $@ $^

# Сборка приложений
%.bin: $(APPSDIR)/%.c
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$*.app.o
	$(LD) $(LDFLAGS_APPS) -o $@ $(OBJDIR)/$*.app.o

# Компиляция C файлов из lib/
$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Компиляция C файлов из kernel/
$(OBJDIR)/%.o: $(KERNELDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Компиляция C файлов из подпапок kernel/ (например, kernel/cpu/)
$(OBJDIR)/%.o: $(KERNELDIR)/*/%.c
	$(CC) $(CFLAGS) $< -o $@

# КОМПИЛЯЦИЯ ВСЕХ ASM ФАЙЛОВ (теперь работает для любого количества .asm)
$(OBJDIR)/%_asm.o: $(KERNELDIR)/%.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) *.bin
