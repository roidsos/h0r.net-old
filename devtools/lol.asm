filler: db "   "; THREE bytes
bpbOEM     db "YoOs    "    ;OEM identifier(idk how to spell this) max 8 byte!!!
bpbBytesPerSector:  dw 512
bpbSectorsPerCluster:   db 1
bpbReservedSectors:  dw 1
bpbNumberOfFATs:  db 2
bpbRootEntries:   dw 224
bpbTotalSectors:  dw 2880
bpbMedia:    db 0xf8
bpbSectorsPerFAT:  dw 9
bpbSectorsPerTrack:     dw 18
bpbHeadsPerCylinder: dw 2
bpbHiddenSectors:  dd 0
bpbTotalSectorsBig:  dd 0
times 510-($-$$) db 0
;dw 0xAA55