%include "constants.inc"

global check_input

extern sys_read, sys_write, sys_poll, sys_exit

section .bss
key_buff resb 8
poll_fd resd 1
poll_events resw 1
poll_revents resw 1

section .rodata
restore_terminal db 27, "[0m", 27, "[?25h", 10
restore_terminal_len equ $ - restore_terminal

section .text
check_input:
    mov dword [poll_fd], STDIN
    mov word [poll_events], POLLIN
    mov word [poll_revents], 0

    mov rdi, poll_fd
    mov rsi, 1
    xor rdx, rdx
    call sys_poll
    cmp rax, 0
    jle .done

    test word [poll_revents], POLLIN
    jz .done

    mov rdi, STDIN
    mov rsi, key_buff
    mov rdx, 1
    call sys_read
    cmp byte [key_buff], 'q'
    je .quit

.done:
    ret

.quit:
    mov rdi, STDOUT
    mov rsi, restore_terminal
    mov rdx, restore_terminal_len
    call sys_write

    xor rdi, rdi
    call sys_exit
