
# h0r.net
A kernel IDK

## What is h0r.net?
why do you care? it is litterally insignificant
## How to get it running?
### On Arch Linux and similar:
  Install packages from normal repositories: [libisoburn](https://archlinux.org/packages/extra/x86_64/libisoburn/) [nasm](https://archlinux.org/packages/extra/x86_64/nasm/)
 [build-essentials](https://archlinux.org/packages/extra/x86_64/build-essentials/)</br>
In case any package is missing please open up an issue!

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
- [ ] VMM (4 level)
- [x] Heap
### ACPI
- [X] ACPI
- [x] LAPIC
- [x] IOAPIC
- [ ] LAI

### Device drivers
#### Audio
- [x] PC speaker
- [ ] AC97

#### I/O
- [x] PS/2 Keyboard
- [ ] PS/2 Mouse
- [x] COM
<!--
#### Storage
- [ ] IDE
- [ ] SATA
- [ ] NVMe
#### Network
- [ ] RTL8139
- [ ] RTL8169
- [ ] E1000
#### USB
- [ ] UHCI
- [ ] OHCI
- [ ] EHCI
- [ ] XHCI 
-->
### Timers
- [x] HPET
- [x] RTC
- [x] LAPIC Timer
<!--
### Tasking
- [ ] SMP
- [ ] Scheduler
- [ ] Signals
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
