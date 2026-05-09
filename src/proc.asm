%include "constants.inc"

global read_cpuinfo, read_meminfo, read_uptime
global cpu_buff, mem_buff, uptime_buff
global cpu_len, mem_len, uptime_len

extern sys_open, sys_read, sys_close

section .bss
cpu_buff resb CPU_BUF_SIZE
mem_buff resb MEM_BUF_SIZE
uptime_buff resb UPTIME_BUF_SIZE
cpu_len resq 1
mem_len resq 1
uptime_len resq 1

section .text

read_cpuinfo:
    mov rdi, cpuinfo_path
    xor rsi, rsi
    call sys_open
    test rax, rax
    js .error

    mov rdi, rax
    mov r8, rax
    mov rsi, cpu_buff
    mov rdx, CPU_BUF_SIZE
    call sys_read
    mov [cpu_len], rax

    mov rdi, r8
    call sys_close
    ret
.error:
    mov qword [cpu_len], 0
    ret

read_meminfo:
    mov rdi, meminfo_path
    xor rsi, rsi
    call sys_open
    test rax, rax
    js .error

    mov rdi, rax
    mov r8, rax
    mov rsi, mem_buff
    mov rdx, MEM_BUF_SIZE
    call sys_read
    mov [mem_len], rax

    mov rdi, r8
    call sys_close
    ret
.error:
    mov qword [mem_len], 0
    ret

read_uptime:
    mov rdi, uptime_path
    xor rsi, rsi
    call sys_open
    test rax, rax
    js .error

    mov rdi, rax
    mov r8, rax
    mov rsi, uptime_buff
    mov rdx, UPTIME_BUF_SIZE
    call sys_read
    mov [uptime_len], rax

    mov rdi, r8
    call sys_close
    ret
.error:
    mov qword [uptime_len], 0
    ret

section .rodata
cpuinfo_path db "/proc/cpuinfo", 0
meminfo_path db "/proc/meminfo", 0
uptime_path db "/proc/uptime", 0
