

all:

	cd boot/stage1 && make && cd .. && cd ..

	cd boot/stage2 && make && cd .. && cd ..

	cd kernel && make && cd .. 

	rm os.img
	dd if=/dev/zero of=os.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" os.img
	dd if=bin/boot.bin of=os.img conv=notrunc
	mcopy -i os.img bin/stage2.bin "::stage2.bin"
	mcopy -i os.img bin/kernel.bin "::kernel.bin"


never:
	ld -T link.ld
	nasm -f bin -o kernel.bin boot/kernel.asm
	nasm -f bin -o kernel.bin boot/zeroes.asm
	gcc -o kernel.o -c kernel.c
	ld -T link.ld