build:
	nasm -f bin boot_sect.asm -o boot_sect.bin
run:
	qemu-system-x86_64 boot_sect.bin