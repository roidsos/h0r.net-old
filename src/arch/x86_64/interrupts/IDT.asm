[bits 64]

; void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* idtDescriptor);
global IDT_Load
global ISR_ignore
IDT_Load:

    ; make new call frame
    push rbp             ; save old call frame
    mov rbp, rsp         ; initialize new call frame
    
    ; load idt
    mov rax, [rbp + 8]
    lidt [rax]

    ; restore old call frame
    mov rsp, rbp
    pop rbp
    ret
