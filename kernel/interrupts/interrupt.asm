global isr1
global isr0
global isrIgnore
global loadidt
extern keyint
extern Tick
extern _idt

idtdesc:
dw 4095
dq _idt

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

loadidt:
lidt [idtdesc]
sti
ret

isr0:
PUSHALL
cld
call Tick
POPALL
isrIgnore:
iretq

isr1:
PUSHALL
cld
call keyint
POPALL
iretq