[bits 64]
extern DefaultGDT
gdtr:
dw 47
dq DefaultGDT

load_default_gdt:   
    lgdt [gdtr]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq
global load_default_gdt
