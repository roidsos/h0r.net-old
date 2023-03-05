%include "asm/macros.inc"
global x86_Read_From_Drive

x86_Read_From_Drive;
go2realmode

go2protectedmode
ret
