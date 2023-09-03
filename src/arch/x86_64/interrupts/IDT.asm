global IDT_Load
extern g_IDT

idt_desc:
dw 4095
dq g_IDT

IDT_Load:
    lidt [idt_desc]
    ret
