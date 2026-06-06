#ifndef SYSMON_H
#define SYSMON_H

#define STDIN_FD 0
#define STDOUT_FD 1

#define O_RDONLY 0
#define POLLIN 1

#define CPU_BUF_SIZE 1024
#define MEM_BUF_SIZE 512
#define UPTIME_BUF_SIZE 64
#define REFRESH_NSEC 250000000L

typedef unsigned long usize;
typedef long isize;

struct timespec64 {
    long tv_sec;
    long tv_nsec;
};

struct pollfd64 {
    int fd;
    short events;
    short revents;
};

extern char cpu_buff[CPU_BUF_SIZE];
extern char mem_buff[MEM_BUF_SIZE];
extern char uptime_buff[UPTIME_BUF_SIZE];
extern long cpu_len;
extern long mem_len;
extern long uptime_len;

long sys_write(long fd, const void *buf, long count);
long sys_read(long fd, void *buf, long count);
long sys_open(const char *path, long flags);
long sys_close(long fd);
long sys_poll(struct pollfd64 *fds, long nfds, long timeout);
long sys_nanosleep(const struct timespec64 *req, struct timespec64 *rem);
void sys_exit(long code);

void read_cpuinfo(void);
void read_meminfo(void);
void read_uptime(void);
void render_screen(void);
void check_input(void);

#endif
