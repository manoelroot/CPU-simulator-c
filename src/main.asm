%include "constants.inc"

global _start
extern read_cpuinfo, read_meminfo, read_uptime
extern render_screen, check_input, sys_nanosleep

section .text
_start:
main_loop:
    call read_cpuinfo
    call read_meminfo
    call read_uptime
    call render_screen
    call check_input

    mov rdi, refresh_time
    xor rsi, rsi
    call sys_nanosleep

    jmp main_loop

section .data
refresh_time dq 0, REFRESH_NSEC
