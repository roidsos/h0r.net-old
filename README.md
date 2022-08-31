
# h0r.net
an open-source kernel for BlindsOS</br>
BlindsOS discord: [https://discord.gg/BlindsOS](https://discord.gg/8eEQqKGqk2)</br>
h0r.net discord: [https://discord.gg/h0r.net](https://discord.gg/hcg9F7hhEM)</br>

## What is h0r.net?
h0r.net is a W.I.P. kernel witch is non-profit and open source.</br>
Current features:</br>
-printing strings with color</br>
-memory managment</br>
-IDTs</br>
-keyboard input</br>
-VGA support</br>

## TODO(stolen from  ilobilo )

### System
- [ ] GDT
- [x] IDT
- [ ] TSS
- [ ] PCI
- [ ] PCIe
- [ ] MSI
- [ ] MSI-X
- [ ] Modules

### Memory
- [ ] PMM
- [ ] VMM (5 and 4 level)
- [x] Heap

### ACPI
- [ ] ACPI
- [ ] LAPIC
- [ ] IOAPIC
- [ ] LAI

### Device drivers
#### Audio
- [X] PC speaker
- [ ] AC97
- [ ] Intel High Definition Audio
- [ ] SB16
#### I/O
- [X] PS/2 Keyboard
- [ ] PS/2 Mouse
- [ ] COM
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

### Timers
- [ ] HPET
- [x] PIT
- [ ] RTC
- [ ] LAPIC Timer

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

## How to get it running?
### On Arch Linux and similar:
  Install packages from aur: [x86_64-elf-gcc](https://aur.archlinux.org/packages/x86_64-elf-gcc) [x86_64-elf-binutils](https://aur.archlinux.org/packages/x86_64-elf-binutils)</br>
  Install packages from normal repositories: [libosburn](https://archlinux.org/packages/extra/x86_64/libisoburn/) [grub](https://archlinux.org/packages/core/x86_64/grub/) [mtools](https://archlinux.org/packages/extra/x86_64/mtools/)</br>
In case any package is missing please open up an issue!</br>
### On any other linux distro:
  *Figure it out I wont deal with debian*
