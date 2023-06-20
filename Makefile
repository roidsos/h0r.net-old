all:
# builds the kernel
	cd src && make && cd .. 
# copies the kernel
	cp src/kernel.bin iso/boot/kernel.bin
# build limine
	$(MAKE) -C limine
#make an ISO
	rm -rf iso_root
	mkdir -p iso_root
	cp src/kernel.bin \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	mkdir -p iso_root/EFI/BOOT
	cp limine/BOOT*.EFI iso_root/EFI/BOOT/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o os.iso
	limine/limine-deploy os.iso
	rm -rf iso_root

run: all
	qemu-system-x86_64 -cdrom os.iso -debugcon stdio -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0
# runs using qemu

debug: all
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

# standard qemu debug
debugr: all
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

#remote debug using GDB
clean:
# JUST DELETES JUNK LIKE OBJECT FILES - fuck capslock
	-rm -rf kernel/*/*.o kernel/kernel.bin iso/boot/kernel.bin
