# OSdev-project
OS development followed by OSdev.wiki

Follow this page to buld cross compiler platform:
https://wiki.osdev.org/GCC_Cross-Compiler

### Building instructions:

Assemble boot.s
`i686-elf-as boot.s -o boot.o`

Compile kernel.c
`i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra`

Link using linker.ld
`i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc`

### Making A bootable ISO:
`mkdir -p isodir/boot/grub`

`cp myos.bin isodir/boot/myos.bin`

`cp grub.cfg isodir/boot/grub/grub.cfg`

`grub-mkrescue -o myos.iso isodir`

### Running using QEMU

`qemu-system-i386 -cdrom myos.iso`
or
`qemu-system-i386 -kernel myos.bin`
to run the kernel without bootloader

This is a personal project dedicated to study core os concepts and development.
