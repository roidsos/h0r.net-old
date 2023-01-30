global loadgdt
extern _gdt

gdtdesc:
dw 4095
dq _gdt

%include "asm/macros.inc"

loadgdt:
lidt [gdtdesc]
ret