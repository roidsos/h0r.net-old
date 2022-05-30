

all:
	nasm -f bin -o boot.bin boot/boot.asm

	nasm -f bin -o kernel.bin boot/kernel.asm

	rm os.img
	dd if=/dev/zero of=os.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" os.img
	dd if=boot.bin of=os.img conv=notrunc
	mcopy -i os.img kernel.bin "::stage2.bin"


never:
	ld -T link.ld
	nasm -f bin -o kernel.bin boot/kernel.asm
	nasm -f bin -o kernel.bin boot/zeroes.asm
	gcc -o kernel.o -c kernel.c
	ld -T link.ld