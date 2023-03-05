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
%macro go2realmode 0
    [bits 64]
    go2realmode:
        cli
        ;disable paging
        mov rax, cr0
        and rax, ~0x80000000
        mov cr0, rax

    ;copied from osdev.org
    reloadSegments:
        ;disable protected mode
        mov rax, cr0
        and ax, ~1
        mov cr0, rax

    ; Reload CS register:
        PUSH 0x18                      ; Push code segment to stack, 0x08 is a stand-in for your code segment
        LEA RAX, [rel .reload_CS]   ; Load address of .reload_CS into RAX here is the problem? maybe?
        PUSH RAX                    ; Push this value to the stack
        RETFQ                       ; Perform a far return, RETFQ or LRETQ depending on syntax

    .reload_CS:

    [bits 16]
    .rmode16:
        ; Reload data segment registers
        MOV   AX, 0x20 ; 0x10 is a stand-in for your data segment
        MOV   DS, AX
        MOV   ES, AX
        MOV   FS, AX
        MOV   GS, AX
        MOV   SS, AX
%endmacro

%macro go2protectedmode 0
extern loadidt
    [bits 16]
    go2protectedmode:
        jmp 0x8:.pmode64


    [bits 32]
    .pmode64:
        ; Reload data segment registers
        MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
        MOV   DS, AX
        MOV   ES, AX
        MOV   FS, AX
        MOV   GS, AX
        MOV   SS, AX

        ;enable protected mode
        mov rax, cr0
        or rax, 1
        mov cr0, rax
        

        ;enable paging
        mov rax, cr0
        or rax, 0x80000000
        mov cr0, rax
        
        call loadidt
        sti
%endmacro