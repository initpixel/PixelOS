#!/bin/bash

RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
CYAN='\033[36m'
NC='\033[0m'

print_info() {
    local message="$1"
    echo -e "${CYAN}[ INFO ]${NC} ${message}"
}

print_ok() {
    local message="$1"
    echo -e "${GREEN}[  OK  ]${NC} ${message}"
}

print_failed() {
    local message="$1"
    echo -e "${RED}[ FAILED ]${NC} ${message}"
    exit 1
}

check_error() {
    if [ $? -ne 0 ]; then
        print_failed "$1"
    fi
}

OBJ_DIR="objectFiles"
OBJ_FILES="$OBJ_DIR/kasm.o $OBJ_DIR/screen.o $OBJ_DIR/stdio.o $OBJ_DIR/ascii_art.o $OBJ_DIR/io.o $OBJ_DIR/keyboard.o $OBJ_DIR/kc.o"
echo -e "$NC"

echo -e "$GREEN========== Starting Kernel build... ==========$NC"

echo -e "$NC"

nasm -f elf32 kernel.asm -o objectFiles/kasm.o

echo -e "$NC"

echo -e "$GREEN========== Library compiling... ==========$NC"

echo -e "$NC"

libs=(
    "lib/screen.c screen.o"
    "lib/stdio.c stdio.o"
    "lib/ascii_art.c ascii_art.o"
    "lib/io.c io.o"
    "lib/keyboard.c keyboard.o"
)

for lib in "${libs[@]}"; do
    src=$(echo $lib | cut -d' ' -f1)
    bin_name=$(echo $lib | cut -d' ' -f2)
    obj_files=$(echo $lib | cut -d' ' -f2)

    print_info "Compiling $src => $OBJ_DIR/$bin_name..."
    gcc -m32 -ffreestanding -Iinclude -c $src -o $OBJ_DIR/$bin_name
    check_error "Compilation of $src failed"
    print_ok "$bin_name compiled successfully"
done

echo -e "$NC"

echo -e "$GREEN========== Kernel Compilation ==========$NC"

echo -e "$NC"

print_info "Kernel Compilation"
gcc -m32 -ffreestanding -Iinclude -c kernel.c -o $OBJ_DIR/kc.o
check_error "Kernel is not compiled"
print_ok "Kernel compiled"

echo -e "$NC"

echo -e "$GREEN========== Linking ==========$NC"

echo -e "$NC"

print_info "Linking"
ld -m elf_i386 -T link.ld -o kernel $OBJ_FILES
check_error "link.ld not working"
print_ok "link.ld worked"

echo -e "$NC"

echo -e "$GREEN========== Build completed successfully! ==========$HC"

echo -e "$NC"