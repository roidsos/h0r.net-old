
# h0r.net
an open-source kernel</br>

## What is h0r.net?
why do you care? it is litterally insignificant
## How to get it running?
### On Arch Linux and similar:
  Install packages from aur: [x86_64-elf-gcc](https://aur.archlinux.org/packages/x86_64-elf-gcc) [x86_64-elf-binutils](https://aur.archlinux.org/packages/x86_64-elf-binutils)</br>
  Install packages from normal repositories: [libisoburn](https://archlinux.org/packages/extra/x86_64/libisoburn/) [grub](https://archlinux.org/packages/core/x86_64/grub/) [mtools](https://archlinux.org/packages/extra/x86_64/mtools/)</br>
In case any package is missing please open up an issue!</br>

## Current features and TODO(stolen from  ilobilo )

### System
- [x] GDT
- [x] IDT
- [X] TSS
- [x] PCI
- [ ] PCIe
- [ ] MSI
- [ ] MSI-X
- [x] Modules

### Memory
- [x] PMM
- [X] VMM (4 level)
- [x] Heap
### ACPI
- [X] ACPI
- [ ] LAPIC
- [ ] IOAPIC
- [ ] LAI


### Device drivers
#### Audio
- [x] PC speaker
- [ ] AC97
- [ ] Intel High Definition Audio
- [ ] SB16

#### I/O
- [X] PS/2 Keyboard
- [ ] PS/2 Mouse
- [x] COM
<!--
#### VMs
- [ ] VMWare Tools
- [ ] VBox Guest Additions
- [ ] Virtio
#### Storage
- [ ] FDC
- [ ] IDE
- [ ] SATA
- [ ] NVMe
- [ ] Virtio block
#### Network
- [ ] RTL8139
- [ ] RTL8169
- [ ] E1000
- [ ] Virtio network
#### USB
- [ ] UHCI
- [ ] OHCI
- [ ] EHCI
- [ ] XHCI 
-->
### Timers
- [ ] HPET
- [x] PIT
- [x] RTC
- [ ] LAPIC Timer

### Tasking
- [ ] SMP
- [x] Scheduler
- [ ] Signals

<!--

### Partition tables
- [ ] MBR
- [ ] GPT 

### Filesystems
- [ ] VFS
- [ ] TMPFS
- [ ] DEVTMPFS
- [ ] PROCFS
- [ ] SYSFS
- [ ] USTAR
- [ ] ILAR
- [ ] Ext2
- [ ] Fat32
- [ ] ISO9660
- [ ] NTFS

### Userspace
- [ ] System calls
- [ ] ELF
- [ ] Userspace
- [ ] Libc
- [ ] Bash
- [ ] DOOM

### Network stack
- [ ] Ethernet
- [ ] ARP
- [ ] IPv4
- [ ] ICMPv4
- [ ] TCP
- [ ] UDP
- [ ] DHCP
- [ ] HTTP
- [ ] Telnet
- [ ] SSL
- [ ] Or just LWIP
-->
