[bits 64]

global load_gdt, load_tss
extern gdt
gdtr:
    dw (0xFF * 8)
    dq gdt

load_tss:
    mov ax, 0x48
    ltr ax
    ret

load_gdt:
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