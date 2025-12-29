#!/bin/bash
make clean
if make; then
    dd if=/dev/zero of=disk.img bs=1M count=20
    mkfs.ext2 -b 1024 -F disk.img 10M
    dd if=shell.bin of=disk.img bs=1M seek=10 conv=notrunc
    echo "Disk image ready. shell.bin is at 10MB offset (sector 20480)."
    qemu-system-i386 -kernel pixelos.bin -drive file=disk.img,format=raw,index=0,media=disk
else
    echo "Build failed!"
fi
