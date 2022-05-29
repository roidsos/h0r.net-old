bits 16
org 0x7C00

mov dl,0h
mov dh,0h
mov ch,0h
mov cl,2
mov bx,0x7e00

read:
mov ah,2
mov al,4
int 13h
jc read

jmp 0x7e00

times 510-($-$$)  db 0
dw 0xAA55