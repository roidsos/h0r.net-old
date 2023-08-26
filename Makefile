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
# runs using qemu
	qemu-system-x86_64 -cdrom os.iso -debugcon stdio -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0

runuefi: all
# runs using qemu
	qemu-system-x86_64 -cdrom os.iso -debugcon stdio -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d -audiodev pa,id=audio0 -machine pcspk-audiodev=audio0 -drive if=pflash,format=raw,unit=0,file="OVMFbin/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="OVMFbin/OVMF_VARS-pure-efi.fd"
debug: all
# standard qemu debug
	qemu-system-x86_64 -no-reboot -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

debugr: all
#remote debug using GDB
	qemu-system-x86_64 -s -S -no-reboot -monitor stdio -d int -no-shutdown -cdrom os.iso -device sb16 -drive file=hdd.img,if=ide,index=0,media=disk,format=raw -boot order=d

clean:
# JUST DELETES JUNK LIKE OBJECT FILES - fuck capslock
	make -C src clean
	rm -rf src/kernel.bin iso/boot/kernel.bin
