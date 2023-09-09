global enable_IDT
extern g_IDT

idt_desc:
dw 4095
dq g_IDT

enable_IDT:
    lidt [idt_desc]
    ret
