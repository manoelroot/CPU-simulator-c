NAME=asm-sysmon
ASM=nasm
ASMFLAGS=-f elf64 -g -F dwarf -Iinclude/
LD=ld

SRC=src/main.asm src/ui.asm src/proc.asm src/syscalls.asm src/input.asm
OBJ=$(SRC:.asm=.o)

all: build/$(NAME)

build/$(NAME): $(OBJ)
	mkdir -p build
	$(LD) -o $@ $(OBJ)

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -f src/*.o build/$(NAME)

.PHONY: all clean
