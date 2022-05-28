bits 16
org 0x7C00


mov dl,0h
mov dh,0h
mov ch,0h
mov cl,2
mov bx,1000h
mov es,bx
mov bx,0h

read:
mov ah,2
mov al,1
int 13h
jc read

mov ax,1000h
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

jmp 1000h:0h

times 510-($-$$)  db 0
dw 0xAA55