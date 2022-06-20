all:
	cd kernel && make && cd .. 

	rm os.iso
	cp kernel/kernel.bin iso/boot/kernel.bin 
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

run: all
	qemu-system-x86_64 -cdrom os.iso

debug: all
	qemu-system-x86_64 -no-reboot -no-shutdown -d int -cdrom os.iso