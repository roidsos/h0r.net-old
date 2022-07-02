all:
	cd kernel && make && cd .. 

	cp kernel/kernel.bin iso/boot/kernel.bin 
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

run: all
	qemu-system-x86_64 -cdrom os.iso -soundhw pcspk

debug: all
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -soundhw pcspk

debugr: all
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -soundhw pcspk