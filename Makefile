all:
# builds the kernel
	cd src && make && cd .. 
# copies the kernel
	cp src/kernel.bin iso/boot/kernel.bin
#makes an ISO
	grub-mkrescue /usr/lib/grub/i386-pc -o os.iso ./iso

run: all
	qemu-system-x86_64 -cdrom os.iso  -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0
# runs using qemu

debug: all
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

# standard qemu debug
debugr: all
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

#remote debug using GDB
clean:
# JUST DELETES JUNK LIKE OBJECT FILES - fuck capslock
	-rm -rf kernel/*/*.o kernel/boot/*.o kernel/util/*.o kernel/drivers/*.o kernel/gdt/*.o kernel/interrupts/*.o kernel/memory/*.o kernel/render/*.o kernel/*.o os.iso .vscode/ kernel/kernel.bin iso/boot/kernel.bin
