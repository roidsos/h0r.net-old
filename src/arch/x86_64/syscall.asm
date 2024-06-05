global syscall_isr
extern syscall_handler
section .text

%macro PUSHA64 0
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rbp
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
%endmacro

%macro POPA64 0
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
%endmacro

; thanks to https://github.com/asterd-og/ZanOS/blob/main/kernel/src/sys/user.asm

syscall_isr:
    swapgs
    mov [gs:0], rsp ; Save user stack in the stack
    mov rsp, [gs:8] ; Kernel stack
    push 0
    push 0 
    PUSHA64

    mov rdi, rsp
    call syscall_handler
    POPA64
    add rsp, 16

    mov [gs:8], rsp
    mov rsp, [gs:0]
    swapgs ; swap again, now kernel gs is the kernel gs again
    o64 sysret