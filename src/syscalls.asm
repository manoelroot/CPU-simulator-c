%include "constants.inc"

global sys_write, sys_exit, sys_read, sys_open, sys_close, sys_poll
global sys_nanosleep

sys_write:
    mov rax, SYS_WRITE
    syscall
    ret

sys_exit:
    mov rax, SYS_EXIT
    syscall
    ret

sys_open:
    mov rax, SYS_OPEN
    syscall
    ret

sys_read:
    mov rax, SYS_READ
    syscall
    ret

sys_close:
    mov rax, SYS_CLOSE
    syscall
    ret

sys_poll:
    mov rax, SYS_POLL
    syscall
    ret

sys_nanosleep:
    mov rax, SYS_NANOSLEEP
    syscall
    ret
