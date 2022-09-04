filler: db "     "; FIVE bytes
bpbOEM:     db "hheellllyyeeaa" ;OEM identifier(idk how to spell this) max 13 byte!!!
dw 0    ;null terminator
bpbBytesPerSector:  dw 512
bpbBytesPerSector2:  dw 512
bpbSectorsPerCluster:   db 1
bpbSectorsPerCluster2:   db 1
bpbReservedSectors:  dw 1
bpbReservedSectors2:  dw 1
bpbNumberOfFATs:  db 2
bpbNumberOfFATs2:  db 2
bpbRootEntries:   dw 224
bpbRootEntries2:   dw 224
bpbTotalSectors:  dw 2880
bpbTotalSectors2:  dw 2880
bpbMedia:    db 0xf8
bpbMedia2:    db 0xf8
bpbSectorsPerFAT:  dw 9
bpbSectorsPerFAT2:  dw 9
bpbSectorsPerTrack:     dw 18
bpbSectorsPerTrack2:     dw 18
bpbHeadsPerCylinder: dw 2
bpbHeadsPerCylinder2: dw 2
bpbHiddenSectors:  dd 0
bpbHiddenSectors2:  dd 0
bpbTotalSectorsBig:  dd 0
bpbTotalSectorsBig2:  dd 0
times 510-($-$$) db 0
;everything is duplicated cuz that is the format for virtual hatd drives for some odd reason
;dw 0xAA55