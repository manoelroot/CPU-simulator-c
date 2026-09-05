#include "sysmon.h"

static const struct timespec64 refresh_time = {0, REFRESH_NSEC};

void _start(void)
{
    for (;;) {
        read_cpuinfo();
        read_cpu_stats();
        read_meminfo();
        read_uptime();
        read_loadavg();
        update_metrics();
        render_screen();
        check_input();
        sys_nanosleep(&refresh_time, 0);
    }
}
