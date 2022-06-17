global isr1
global loadIDT
extern isr1_handler
extern _idt
%macro PUSHALL 0
push rax
push rdx
push rcx
push r8
push r9
push r10
push r11
%endmacro
%macro POPALL 0
pop rax
pop rdx
pop rcx
pop r8
pop r9
pop r10
pop r11
%endmacro

idtdesc:
dw 4095
dw _idt

isr1:
PUSHALL
call isr1_handler
POPALL
iretq

loadIDT:
lidt [idtdesc]
sti
ret