segment .start
extern cstart
global entry

entry:
;call enableA20
in al, 0x92       ; read data through port 0x92 (chipset)
or al, 2          ; mask bit 2
out 0x92, al      ; send data back to chipset

cli

lgdt [GDT_descriptor]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp CODE_SEG:start_protected_mode
jmp $


[bits 32]
start_protected_mode:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

	mov es, ax
	mov fs, ax
	mov gs, ax
    call cstart
jmp $
%include "gdt.inc"
%include "a20.inc"