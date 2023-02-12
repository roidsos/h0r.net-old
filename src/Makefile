# C file discovery
source_files := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard */*/*/*.cpp) $(wildcard */*/*/*/*.cpp) $(wildcard */*/*/*/*/*.cpp)
kobject_files := $(source_files:.cpp=.o)

# asm file discovery
asm_source_files := $(wildcard *.asm) $(wildcard */*.asm) $(wildcard */*/*.asm) $(wildcard */*/*/*.asm) $(wildcard */*/*/*/*.asm) $(wildcard */*/*/*/*/*.asm)
asm_object_files := $(asm_source_files:.asm=.o)
all: build-x86_64

object_files := $(asm_object_files) $(kobject_files)

$(kobject_files):$(source_files)
#compiles a source file with a lot of STFU options cuz I hate debugging
	g++ -I . -fpermissive -ffreestanding -m64 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -c $(patsubst %.o, %.cpp, $@) -o $@

$(asm_object_files):$(asm_source_files)
#assembles the .asm files
	nasm -f elf64 $(patsubst %.o, %.asm, $@) -o $@

build-x86_64: $(object_files)
# this is the linkng phase, it links using a linker script
	ld -n -o  kernel.bin -T linker.ld $^ 
