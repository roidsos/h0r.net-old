    global loadgdt
    extern _gdt
    ;ok I converted it to text using hexdump
    ;go to mem.txt

    gdtdesc:
        dw 4095
        dq _gdt

    %include "asm/macros.inc"

    loadgdt:
    lgdt [gdtdesc] ; this loads the GDT
    call reloadSegments; error here but IDK why
    ret

    ;copied from osdev.org
    reloadSegments:
    ; Reload CS register:
        PUSH 8                      ; Push code segment to stack, 0x08 is a stand-in for your code segment
        LEA RAX, [rel .reload_CS]   ; Load address of .reload_CS into RAX here is the problem? maybe?
        PUSH RAX                    ; Push this value to the stack
        RETFQ                       ; Perform a far return, RETFQ or LRETQ depending on syntax
    .reload_CS:
    ; Reload data segment registers
        MOV   AX, 16 ; 0x10 is a stand-in for your data segment
        MOV   DS, AX
        MOV   ES, AX
        MOV   FS, AX
        MOV   GS, AX
        MOV   SS, AX
        RET