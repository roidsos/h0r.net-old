CC=i386-elf-gcc
LD=i386-elf-ld
ASMFLAGS?=-f elf32
CFLAGS16?= #-4 -d3 -s -wx -ms -zl -zq  -oneatxzh 
ASM=nasm
BUILD_DIR = ../../bin

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/stage2/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.o, $(SOURCES_ASM))

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD) -T linker.ld -o $(BUILD_DIR)/stage2.bin  $(OBJECTS_ASM) $(OBJECTS_C)

$(BUILD_DIR)/stage2/c/%.o: %.c always
	$(CC) $(CFLAGS16) -o $@ -c $<

$(BUILD_DIR)/stage2/asm/%.o: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/stage2/c
	mkdir -p $(BUILD_DIR)/stage2/asm
