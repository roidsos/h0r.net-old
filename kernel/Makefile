source_files := $(wildcard *.c) $(wildcard */*.c)
kobject_files := $(source_files:.c=.o)

asm_source_files := $(wildcard *.asm) $(wildcard */*.asm)
asm_object_files := $(asm_source_files:.asm=.o)
all: build-x86_64

object_files := $(asm_object_files) $(kobject_files)

$(kobject_files):
	x86_64-elf-gcc -I . -c -ffreestanding $(patsubst %.o, %.c, $@) -o $@

$(asm_object_files):
	nasm -f elf64 $(patsubst %.o, %.asm, $@) -o $@

build-x86_64: $(object_files) 
	x86_64-elf-ld -n -o kernel.bin -T linker.ld $^ 
	rm *.o
	rm boot/*.o
