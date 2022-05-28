

all: boot kernel link


boot:
	nasm -f bin -o boot.bin boot.asm
kernel:
	nasm -f bin -o kernel.bin kernel.asm
	
link:
	
	rm os.img
	cat boot.bin kernel.bin >> os.img


never:
	nasm -f bin -o kernel.bin kernel.asm
	nasm -f bin -o kernel.bin zeroes.asm
	gcc -o kernel.o -c kernel.c
	ld -T link.ld