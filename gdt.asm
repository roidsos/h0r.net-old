gdt_nulldesc:
dd 0
dd 0

gdt_text:
dw 0xFFFF       ;limit         
dw 0x0000       ;base             
db 0x00         ;more base        
db 10011010b    ;flags            
db 11001111b    ;flags + limit 
db 0x00         ;even more base
gdt_data:
dw 0xFFFF       ;limit         
dw 0x0000       ;base          
db 0x00         ;more base     
db 10010010b    ;flags         
db 11001111b    ;flags + limit 
db 0x00         ;even more base

gdt_end:

gdtdesc:
    size: dw gdt_end - gdt_nulldesc -1
    addr: dd gdt_nulldesc

    codeseg equ gdt_text - gdt_nulldesc
    dataseg equ gdt_data - gdt_nulldesc

