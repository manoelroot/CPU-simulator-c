#include "sysmon.h"

static const struct timespec64 refresh_time = {0, REFRESH_NSEC};

void _start(void)
{
    for (;;) {
        read_cpuinfo();
        read_meminfo();
        read_uptime();
        render_screen();
        check_input();
        sys_nanosleep(&refresh_time, 0);
    }
}
