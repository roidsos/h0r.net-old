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

%macro SAVETSS 0
    ; Save state
    mov [.tss], rax
    mov [.tss + 8], rbx
    mov [.tss + 16], rcx
    mov [.tss + 24], rdx
    mov [.tss + 32], rdi
    mov [.tss + 40], rsi
    mov rax, [rsp + 24]
    mov [.tss + 48], rax
    mov rax, [rsp + 16]
    mov [.tss + 56], rax
    mov rax, [rsp]
    mov [.tss + 64], rax
    mov rax, [rsp + 32]
    cmp rax, 0
    jg .done_seg
    mov rax, 0x10
.done_seg:
    mov [.tss + 136 + 16 * 16 + 8], rax
    mov rax, [rsp + 8]
    mov [.tss + 136 + 16 * 16 + 0], rax
    mov [.tss + 72], rbp
    mov [.tss + 80], r9
    mov [.tss + 88], r10
    mov [.tss + 96], r11
    mov [.tss + 104], r12
    mov [.tss + 112], r13
    mov [.tss + 120], r14
    mov [.tss + 128], r15
    movdqu [.tss + 136], xmm0
    movdqu [.tss + 136 + 16 * 1], xmm1
    movdqu [.tss + 136 + 16 * 2], xmm2
    movdqu [.tss + 136 + 16 * 3], xmm3
    movdqu [.tss + 136 + 16 * 4], xmm4
    movdqu [.tss + 136 + 16 * 5], xmm5
    movdqu [.tss + 136 + 16 * 6], xmm6
    movdqu [.tss + 136 + 16 * 7], xmm7
    movdqu [.tss + 136 + 16 * 8], xmm8
    movdqu [.tss + 136 + 16 * 9], xmm9
    movdqu [.tss + 136 + 16 * 10], xmm10
    movdqu [.tss + 136 + 16 * 11], xmm11
    movdqu [.tss + 136 + 16 * 12], xmm12
    movdqu [.tss + 136 + 16 * 13], xmm13
    movdqu [.tss + 136 + 16 * 14], xmm14
    movdqu [.tss + 136 + 16 * 15], xmm15
%endmacro

%macro RESTORETSS 0
    add rsp, 36

    ; Restore state
    push qword [rdi + 136 + 16 * 16 + 8]
    push qword [rdi + 48]
    push qword [rdi + 56]
    push qword [rdi + 136 + 16 * 16]
    push qword [rdi + 64]

    mov rbx, [rdi + 8]
    mov rcx, [rdi + 16]
    mov rdx, [rdi + 24]
    mov rsi, [rdi + 40]
    mov rbp, [rdi + 72]
    mov r9, [rdi + 80]
    mov r10, [rdi + 88]
    mov r11, [rdi + 96]
    mov r12, [rdi + 104]
    mov r13, [rdi + 112]
    mov r14, [rdi + 120]
    mov r15, [rdi + 128]
    movdqu xmm0, [rdi + 136]
    movdqu xmm1, [rdi + 136 + 16 * 1]
    movdqu xmm2, [rdi + 136 + 16 * 2]
    movdqu xmm3, [rdi + 136 + 16 * 3]
    movdqu xmm4, [rdi + 136 + 16 * 4]
    movdqu xmm5, [rdi + 136 + 16 * 5]
    movdqu xmm6, [rdi + 136 + 16 * 6]
    movdqu xmm7, [rdi + 136 + 16 * 7]
    movdqu xmm8, [rdi + 136 + 16 * 8]
    movdqu xmm9, [rdi + 136 + 16 * 9]
    movdqu xmm10, [rdi + 136 + 16 * 10]
    movdqu xmm11, [rdi + 136 + 16 * 11]
    movdqu xmm12, [rdi + 136 + 16 * 12]
    movdqu xmm13, [rdi + 136 + 16 * 13]
    movdqu xmm14, [rdi + 136 + 16 * 14]
    movdqu xmm15, [rdi + 136 + 16 * 15]

    mov rax, [rdi + 136 + 16 * 16 + 8]
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax

    mov rax, [rdi]
    mov rdi, [rdi + 32] ; I didn't forget you!
%endmacro
