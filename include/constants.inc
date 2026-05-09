%ifndef CONSTANTS_INC
%define CONSTANTS_INC

%define STDIN 0
%define STDOUT 1

%define SYS_READ 0
%define SYS_WRITE 1
%define SYS_OPEN 2
%define SYS_CLOSE 3
%define SYS_POLL 7
%define SYS_NANOSLEEP 35
%define SYS_EXIT 60

%define O_RDONLY 0
%define POLLIN 1

%define CPU_BUF_SIZE 1024
%define MEM_BUF_SIZE 512
%define UPTIME_BUF_SIZE 64

%define REFRESH_NSEC 250000000

%endif
