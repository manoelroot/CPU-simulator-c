#include "sysmon.h"

char cpu_buff[CPU_BUF_SIZE];
char mem_buff[MEM_BUF_SIZE];
char uptime_buff[UPTIME_BUF_SIZE];
long cpu_len;
long mem_len;
long uptime_len;

static long read_proc_file(const char *path, char *buffer, long capacity)
{
    long fd = sys_open(path, O_RDONLY);
    long bytes_read;

    if (fd < 0) {
        return 0;
    }

    bytes_read = sys_read(fd, buffer, capacity);
    sys_close(fd);

    if (bytes_read < 0) {
        return 0;
    }

    return bytes_read;
}

void read_cpuinfo(void)
{
    cpu_len = read_proc_file("/proc/cpuinfo", cpu_buff, CPU_BUF_SIZE);
}

void read_meminfo(void)
{
    mem_len = read_proc_file("/proc/meminfo", mem_buff, MEM_BUF_SIZE);
}

void read_uptime(void)
{
    uptime_len = read_proc_file("/proc/uptime", uptime_buff, UPTIME_BUF_SIZE);
}
