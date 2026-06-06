#include "sysmon.h"

static char key_buff[8];
static struct pollfd64 stdin_poll;
static const char restore_terminal[] = "\033[0m\033[?25h\n";

void check_input(void)
{
    stdin_poll.fd = STDIN_FD;
    stdin_poll.events = POLLIN;
    stdin_poll.revents = 0;

    if (sys_poll(&stdin_poll, 1, 0) <= 0) {
        return;
    }

    if ((stdin_poll.revents & POLLIN) == 0) {
        return;
    }

    if (sys_read(STDIN_FD, key_buff, 1) <= 0) {
        return;
    }

    if (key_buff[0] == 'q') {
        sys_write(STDOUT_FD, restore_terminal, sizeof(restore_terminal) - 1);
        sys_exit(0);
    }
}
