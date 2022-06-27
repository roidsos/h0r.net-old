section .text
extern _ZN3IDT15HandleInterruptEhy
IRQ_BASE equ 0x20
global _ZN3IDT15InterruptIgnoreEv
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

%macro HANDLEEX 1
global _ZN3IDT25HandleExeptionNumber%1Ev
_ZN3IDT25HandleExeptionNumber%1Ev:
mov [interruptnumber],byte %1
jmp int_handler


%endmacro


%macro HANDLEINT 1
global _ZN3IDT25HandleInterruptNumber%1Ev
_ZN3IDT25HandleInterruptNumber%1Ev:
mov [interruptnumber],byte %1 + IRQ_BASE
jmp int_handler
%endmacro


HANDLEINT 0x00
HANDLEINT 0x01


int_handler:
PUSHALL;push stuff
;push ds
;push es
;push fs
;push gs
mov rdi, rsp;add argument 1
mov rsi, interruptnumber;add argument 2

call _ZN3IDT15HandleInterruptEhy;call the handler

mov rax,rsp;restore the stack pointer

;pop gs;pop stuff
;pop fs
;pop es
;pop ds
POPALL

_ZN3IDT15InterruptIgnoreEv:
iretq




section .data
    interruptnumber: db 0