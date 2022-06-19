source_files := $(wildcard *.cpp) $(wildcard */*.cpp)
kobject_files := $(source_files:.cpp=.o)

asm_source_files := $(wildcard *.asm) $(wildcard */*.asm)
asm_object_files := $(asm_source_files:.asm=.o)
all: build-x86_64

object_files := $(asm_object_files) $(kobject_files)

$(kobject_files):
	x86_64-elf-g++ -I . -fpermissive -ffreestanding -m64 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c $(patsubst %.o, %.cpp, $@) -o $@

$(asm_object_files):
	nasm -f elf64 $(patsubst %.o, %.asm, $@) -o $@

build-x86_64: $(object_files)
	x86_64-elf-ld -n -o kernel.bin -T linker.ld $^ 
	rm $(object_files)
	