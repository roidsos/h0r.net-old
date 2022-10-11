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