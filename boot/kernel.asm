mov ah,09h
mov al,'N'
mov bh,0
mov bl,4
mov cx,1
int 10h
jmp $

jmp protect

%include "boot/gdt.asm"
%include "boot/a20.asm"

protect:
call enableA20
cli
lgdt [gdtdesc]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp 08h:startPM

bits 32
startPM:
mov ax,dataseg
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

mov ebp ,0x900000
mov esp ,ebp

mov [0xb8000], byte 'H'

jmp $

times 2048-($-$$)  db 1