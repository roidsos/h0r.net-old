
jmp protect

%include "gdt.asm"
%include "a20.asm"

protect:
call enableA20
cli
lgdt [gdtdesc]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp codeseg:startPM

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