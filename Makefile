all:
	cd kernel && make && cd .. 

	rm os.iso
	cp kernel/kernel.bin iso/boot/kernel.bin 
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

