


%include "gdt.asm"

protect:
call enableA20
cli
lgdt [gdtdesc]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp codeseg:startPM

enableA20:
in al,0x92
or al,2
out 0x92,al
ret

[bits 32]
startPM:

mov ax,codeseg
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

mov ebp ,0x900000
mov esp ,ebp

mov [0xb8000], byte "H"

jmp $

times 2048-($-$$)  db 1