all:
	cd kernel && make && cd .. 

	cp kernel/kernel.bin iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

run: all
	qemu-system-x86_64 -cdrom os.iso  -device sb16 -drive file=hdd.img,if=ide,index=0,media=cdrom,format=raw

debug: all
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=mbr.bin,if=ide,index=0,media=disk,format=raw

debugr: all
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=mbr.bin,if=ide,index=0,media=disk,format=raw

clean:
	-rm -rf kernel/boot/*.o kernel/util/*.o kernel/drivers/*.o kernel/gdt/*.o kernel/interrupts/*.o kernel/memory/*.o kernel/render/*.o kernel/*.o os.iso .vscode/ kernel/kernel.bin iso/boot/kernel.bin