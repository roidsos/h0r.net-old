;!!!Warning this does not work in UEFI mode!!!
global DetectMemory

%macro x86_EnterRealMode 0
;    [bits 32]
;    jmp word 18h:.pmode16         ; 1 - jump to 16-bit protected mode segment

;.pmode16:
    [bits 16]
    ; 2 - disable protected mode bit in cr0
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    ; 3 - jump to real mode
    jmp word 00h:.rmode

.rmode:
    ; 4 - setup segments
    mov ax, 0
    mov ds, ax
    mov ss, ax

    ; 5 - enable interrupts
    sti

%endmacro


%macro x86_EnterProtectedMode 0
    cli

    ; 4 - set protection enable flag in CR0
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ; 5 - far jump into protected mode
    jmp dword 08h:.pmode


.pmode:
    ; we are now in protected mode!
    [bits 32]
    
    ; 6 - setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

%endmacro

MemoryRegionCount:
 db 0

DetectMemory:

x86_EnterRealMode

[bits 16]
mov ax,0
mov es,ax
mov di,0x5000
mov edx,0x534D4150
xor ebx,ebx
.repeat:
    mov eax,0xE820
    mov ecx,24
    int 0x15
    cmp ebx,0
    je .finished

    add di,24
    inc byte [MemoryRegionCount]
    jmp .repeat
    .finished:
    x86_EnterProtectedMode

    [bits 32]

    ret