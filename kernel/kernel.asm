
mov [0xb8000], byte 'H'

jmp $

times 2048-($-$$)  db 1