#!/bin/bash

# Создаем пустой файл размером 10 МБ
dd if=/dev/zero of=disk.img bs=1M count=10

# Форматируем его в ext2 (ответь 'y', если спросит про файл)
mkfs.ext2 disk.img

qemu-system-i386 -kernel pixelos.bin -hda disk.img