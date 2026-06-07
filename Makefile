NAME=asm-sysmon
CC=gcc
ASM=nasm
ASMFLAGS=-f elf64 -g -F dwarf -Iinclude/
CFLAGS=-std=c11 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -Iinclude
LD=gcc
LDFLAGS=-nostdlib -no-pie

CSRC=src/core/main.c src/core/proc.c src/ui/ui.c src/input/input.c
ASMSRC=src/syscalls/syscalls.asm
OBJ=$(patsubst src/%.c,build/objs/%.o,$(CSRC)) $(patsubst src/%.asm,build/objs/%.o,$(ASMSRC))

all: build/$(NAME)

build/$(NAME): $(OBJ)
	mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

build/objs/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/objs/%.o: src/%.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf build/objs build/$(NAME)

.PHONY: all clean
