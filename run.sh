#!/bin/bash
make clean
if make; then
    dd if=/dev/zero of=disk.img bs=1M count=10
    mkfs.ext2 -F disk.img
    debugfs -w -R "write hello.bin hello.bin" disk.img
    qemu-system-i386 -kernel pixelos.bin \
                 -drive file=disk.img,format=raw,index=0,media=disk \
                 -d int -no-reboot

else
    echo "Build failed, check your code!"
fi
