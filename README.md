
# h0r.net
an open-source kernel(rewrite from the ground up)</br>
discord: [https://discord.gg/h0r.net](https://discord.gg/hcg9F7hhEM)</br>

## What is h0r.net?
h0r.net is a W.I.P. kernel witch is non-profit and open source.</br>
the h0r.net project was started just for fun, but Im planning to use it.</br>
## How to get it running?
## on debian
2 commands:</br>
´sh .devcontainer/install.sh´</br>
´make run´

## Current features and TODO(stolen from  ilobilo )

### System
- [x] GDT
- [x] IDT
- [ ] TSS
- [x] PCI
- [ ] PCIe
- [ ] MSI
- [ ] MSI-X
- [x] Modules

### Memory
- [x] PMM
- [ ] VMM (5 and 4 level)
- [x] Heap
<!--
### ACPI
- [ ] ACPI
- [ ] LAPIC
- [ ] IOAPIC
- [ ] LAI

### Device drivers
#### Audio
- [ ] PC speaker
- [ ] AC97
- [ ] Intel High Definition Audio
- [ ] SB16
-->
#### I/O
- [ ] PS/2 Keyboard
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
