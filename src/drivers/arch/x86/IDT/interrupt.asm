global isrC
global isr1
global isr0
global isrIgnore
global loadidt
extern keyint
extern musint
extern Tick
extern _idt

idtdesc:
dw 4095
dq _idt

%include "asm/macros.inc"

loadidt:
lidt [idtdesc]
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

isrC:
PUSHALL
cld
call musint
POPALL
iretq