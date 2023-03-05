%include "asm/macros.inc"
global x86_Read_From_Drive

x86_Read_From_Drive;
go2realmode
[bits 16]
lidt [0]
sti
mov ah, 02h
mov bh, 00h
mov dh, 00h
mov dl, 00h
int 10h
cli
[bits 64]
go2protectedmode
ret
