all:
	-mkdir iso/
	-mkdir iso/boot/
	-mkdir iso/boot/grub/
	-echo "set timeout=0\nset default=0\n\nmenuentry "h0r.net" {\nmultiboot2 /boot/kernel.bin\nboot\n}" > iso/boot/grub/grub.cfg
	cd kernel && make && cd .. 

	cp kernel/kernel.bin iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

run: all
	qemu-system-x86_64 -cdrom os.iso -soundhw pcspk

debug: all
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -soundhw pcspk

debugr: all
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -soundhw pcspk

clean:
	-rm -rf kernel/boot/*.o kernel/drivers/*.o kernel/gdt/*.o kernel/interrupts/*.o kernel/memory/*.o kernel/render/*.o kernel/*.o os.iso .vscode/ kernel/kernel.bin iso/boot/kernel.bin bin/ iso/