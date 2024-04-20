all:
# builds the kernel
	cd src && make && cd .. 
# build limine
	$(MAKE) -C limine
#make an ISO
	mkdir -p iso
	cp src/kernel.bin \
		cfg/limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso/
	mkdir -p iso/EFI/BOOT
	cp limine/BOOT*.EFI iso/EFI/BOOT/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso -o os.iso
	limine/limine-deploy os.iso

override CFILES := $(shell find -L src -type f -name '*.c')
format:
	clang-format -i ${CFILES}

run: all
# runs using qemu
	qemu-system-x86_64 -cdrom os.iso -m 256M -serial file:hornet.log

runuefi: all
# runs using qemu
	qemu-system-x86_64-cdrom os.iso -m 256M -serial file:hornet.log -drive if=pflash,format=raw,unit=0,file="OVMFbin/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="OVMFbin/OVMF_VARS-pure-efi.fd"
debug: all
# standard qemu debug
	qemu-system-x86_64 -no-reboot -serial file:hornet.log -d int -no-shutdown -cdrom os.iso -m 256M

debugr: all
#remote debug using GDB
	qemu-system-x86_64 -s -S -no-reboot -serial file:hornet.log -monitor stdio -d int -no-shutdown -cdrom os.iso -m 256M 

debugruefi: all
#remote debug using GDB
	qemu-system-x86_64 -s -S -no-reboot -serial file:hornet.log -monitor stdio -d int -no-shutdown -cdrom os.iso -m 256M -drive if=pflash,format=raw,unit=0,file="OVMFbin/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="OVMFbin/OVMF_VARS-pure-efi.fd"
clean:
# JUST DELETES JUNK LIKE OBJECT FILES - fuck capslock
	make -C src clean
	rm -rf src/kernel.bin iso/boot/kernel.bin