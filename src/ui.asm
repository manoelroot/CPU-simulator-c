%include "constants.inc"

global render_screen

extern sys_write
extern cpu_buff, mem_buff, uptime_buff
extern cpu_len, mem_len, uptime_len

section .rodata
clear_screen db 27, "[2J", 27, "[H", 27, "[?25l"
clear_screen_len equ $ - clear_screen

reset_color db 27, "[0m"
reset_color_len equ $ - reset_color

cyan_bold db 27, "[1;36m"
cyan_bold_len equ $ - cyan_bold

green_bold db 27, "[1;32m"
green_bold_len equ $ - green_bold

yellow_bold db 27, "[1;33m"
yellow_bold_len equ $ - yellow_bold

dim_color db 27, "[2m"
dim_color_len equ $ - dim_color

top_border db "+------------------------------------------------------------------------------+", 10
top_border_len equ $ - top_border

header db "| ASM-SysMon v0.2                      /proc live monitor                     |", 10
header_len equ $ - header

divider db "+------------------------------------------------------------------------------+", 10
divider_len equ $ - divider

cpu_box_top db "| [ CPU ]                                                                      |", 10
cpu_box_top_len equ $ - cpu_box_top

mem_box_top db "| [ MEMORIA ]                                                                  |", 10
mem_box_top_len equ $ - mem_box_top

uptime_box_top db "| [ UPTIME ]                                                                   |", 10
uptime_box_top_len equ $ - uptime_box_top

line_prefix db "|   "
line_prefix_len equ $ - line_prefix

line_suffix db " |", 10
line_suffix_len equ $ - line_suffix

blank_line db "|                                                                              |", 10
blank_line_len equ $ - blank_line

footer db "| q + Enter: sair                                                              |", 10
footer_len equ $ - footer

bottom_border db "+------------------------------------------------------------------------------+", 10
bottom_border_len equ $ - bottom_border

status_prefix db "| Atualizando ["
status_prefix_len equ $ - status_prefix

status_frames db "|", "/", "-", 92

status_suffix db "]  leitura direta de /proc via syscalls                         |", 10
status_suffix_len equ $ - status_suffix

cpu_model_key db "model name"
cpu_model_key_len equ $ - cpu_model_key

mem_total_key db "MemTotal:"
mem_total_key_len equ $ - mem_total_key

mem_available_key db "MemAvailable:"
mem_available_key_len equ $ - mem_available_key

section .bss
status_frame_index resb 1

section .text

render_screen:
    mov rdi, STDOUT
    mov rsi, clear_screen
    mov rdx, clear_screen_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, cyan_bold
    mov rdx, cyan_bold_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, top_border
    mov rdx, top_border_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, header
    mov rdx, header_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, divider
    mov rdx, divider_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, reset_color
    mov rdx, reset_color_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, cpu_box_top
    mov rdx, cpu_box_top_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, green_bold
    mov rdx, green_bold_len
    call sys_write

    mov rsi, cpu_buff
    mov rdx, [cpu_len]
    mov r8, cpu_model_key
    mov r9, cpu_model_key_len
    call write_matching_line

    mov rdi, STDOUT
    mov rsi, reset_color
    mov rdx, reset_color_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, blank_line
    mov rdx, blank_line_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, mem_box_top
    mov rdx, mem_box_top_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, yellow_bold
    mov rdx, yellow_bold_len
    call sys_write

    mov rsi, mem_buff
    mov rdx, [mem_len]
    mov r8, mem_total_key
    mov r9, mem_total_key_len
    call write_matching_line

    mov rsi, mem_buff
    mov rdx, [mem_len]
    mov r8, mem_available_key
    mov r9, mem_available_key_len
    call write_matching_line

    mov rdi, STDOUT
    mov rsi, reset_color
    mov rdx, reset_color_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, blank_line
    mov rdx, blank_line_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, uptime_box_top
    mov rdx, uptime_box_top_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, uptime_buff
    mov rdx, [uptime_len]
    call write_data_line

    mov rdi, STDOUT
    mov rsi, blank_line
    mov rdx, blank_line_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, dim_color
    mov rdx, dim_color_len
    call sys_write

    call write_status_line

    mov rdi, STDOUT
    mov rsi, footer
    mov rdx, footer_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, bottom_border
    mov rdx, bottom_border_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, reset_color
    mov rdx, reset_color_len
    call sys_write
    ret

; rsi = buffer, rdx = tamanho, r8 = chave, r9 = tamanho da chave
write_matching_line:
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov rbx, rsi
    lea r12, [rsi + rdx]

.next_line:
    cmp rbx, r12
    jae .done

    mov r13, rbx
    xor r14, r14

.compare_key:
    cmp r14, r9
    je .match
    lea r15, [rbx + r14]
    cmp r15, r12
    jae .done
    mov al, [r15]
    cmp al, [r8 + r14]
    jne .skip_line
    inc r14
    jmp .compare_key

.skip_line:
    cmp rbx, r12
    jae .done
    cmp byte [rbx], 10
    je .advance_line
    inc rbx
    jmp .skip_line

.advance_line:
    inc rbx
    jmp .next_line

.match:
    mov r15, r13

.find_line_end:
    cmp r15, r12
    jae .write_line
    cmp byte [r15], 10
    je .include_newline
    inc r15
    jmp .find_line_end

.include_newline:
    jmp .write_line

.write_line:
    mov rdi, STDOUT
    mov rsi, line_prefix
    mov rdx, line_prefix_len
    call sys_write

    mov rdi, STDOUT
    mov rsi, r13
    mov rdx, r15
    sub rdx, r13
    call sys_write

    mov rdi, STDOUT
    mov rsi, line_suffix
    mov rdx, line_suffix_len
    call sys_write

.done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

write_status_line:
    mov rdi, STDOUT
    mov rsi, status_prefix
    mov rdx, status_prefix_len
    call sys_write

    movzx rax, byte [status_frame_index]
    and rax, 3
    mov rsi, status_frames
    add rsi, rax
    mov rdi, STDOUT
    mov rdx, 1
    call sys_write

    inc byte [status_frame_index]
    and byte [status_frame_index], 3

    mov rdi, STDOUT
    mov rsi, status_suffix
    mov rdx, status_suffix_len
    call sys_write
    ret

; rsi = buffer, rdx = tamanho. Escreve somente a primeira linha.
write_data_line:
    push rbx
    push r12

    mov rbx, rsi
    lea r12, [rsi + rdx]

.find_end:
    cmp rbx, r12
    jae .write
    cmp byte [rbx], 10
    je .write
    inc rbx
    jmp .find_end

.write:
    mov rdi, STDOUT
    mov rsi, line_prefix
    mov rdx, line_prefix_len
    call sys_write

    mov rdi, STDOUT
    mov rdx, rbx
    sub rdx, uptime_buff
    mov rsi, uptime_buff
    call sys_write

    mov rdi, STDOUT
    mov rsi, line_suffix
    mov rdx, line_suffix_len
    call sys_write

    pop r12
    pop rbx
    ret
