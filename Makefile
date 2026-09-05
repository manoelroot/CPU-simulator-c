NAME=asm-sysmon
CC=gcc
ASM=nasm
ASMFLAGS=-f elf64 -g -F dwarf -Iinclude/
CFLAGS=-std=c11 -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -fno-pic -fno-pie -Iinclude
LD=gcc
LDFLAGS=-nostdlib -no-pie

CSRC=src/main.c src/ui.c src/proc.c src/metrics.c src/input.c
ASMSRC=src/syscalls.asm
OBJ=$(CSRC:.c=.o) $(ASMSRC:.asm=.o)

all: build/$(NAME)

build/$(NAME): $(OBJ)
	mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -f src/*.o build/$(NAME)

.PHONY: all clean
