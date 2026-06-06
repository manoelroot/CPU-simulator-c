#include "sysmon.h"

#define WRITE_LITERAL(value) sys_write(STDOUT_FD, value, sizeof(value) - 1)

static const char clear_screen[] = "\033[2J\033[H\033[?25l";
static const char reset_color[] = "\033[0m";
static const char cyan_bold[] = "\033[1;36m";
static const char green_bold[] = "\033[1;32m";
static const char yellow_bold[] = "\033[1;33m";
static const char dim_color[] = "\033[2m";

static const char top_border[] = "+------------------------------------------------------------------------------+\n";
static const char header[] = "| ASM-SysMon v0.3                 C core + Assembly syscalls                   |\n";
static const char divider[] = "+------------------------------------------------------------------------------+\n";
static const char cpu_box_top[] = "| [ CPU ]                                                                      |\n";
static const char mem_box_top[] = "| [ MEMORIA ]                                                                  |\n";
static const char uptime_box_top[] = "| [ UPTIME ]                                                                   |\n";
static const char line_prefix[] = "|   ";
static const char line_suffix[] = " |";
static const char newline[] = "\n";
static const char blank_line[] = "|                                                                              |\n";
static const char footer[] = "| q + Enter: sair                                                              |\n";
static const char bottom_border[] = "+------------------------------------------------------------------------------+\n";
static const char status_prefix[] = "| Atualizando [";
static const char status_frames[] = "|/-\\";
static const char status_suffix[] = "]  UI C minimalista, syscalls Linux em Assembly                  |\n";

static unsigned char status_frame_index;

static long key_matches(const char *line, const char *end, const char *key, long key_len)
{
    long index;

    for (index = 0; index < key_len; index++) {
        if (line + index >= end || line[index] != key[index]) {
            return 0;
        }
    }

    return 1;
}

static void write_cropped(const char *data, long len, long max_len)
{
    if (len > max_len) {
        len = max_len;
    }

    if (len > 0) {
        sys_write(STDOUT_FD, data, len);
    }
}

static void write_data_line(const char *data, long len)
{
    long line_len = 0;

    while (line_len < len && data[line_len] != '\n') {
        line_len++;
    }

    WRITE_LITERAL(line_prefix);
    write_cropped(data, line_len, 72);
    WRITE_LITERAL(line_suffix);
    WRITE_LITERAL(newline);
}

static void write_matching_line(const char *buffer, long len, const char *key, long key_len)
{
    const char *cursor = buffer;
    const char *end = buffer + len;

    while (cursor < end) {
        const char *line_start = cursor;
        const char *line_end = cursor;

        while (line_end < end && *line_end != '\n') {
            line_end++;
        }

        if (key_matches(line_start, end, key, key_len)) {
            write_data_line(line_start, line_end - line_start);
            return;
        }

        cursor = line_end;
        if (cursor < end && *cursor == '\n') {
            cursor++;
        }
    }
}

static void write_status_line(void)
{
    char frame = status_frames[status_frame_index & 3];

    WRITE_LITERAL(status_prefix);
    sys_write(STDOUT_FD, &frame, 1);
    WRITE_LITERAL(status_suffix);
    status_frame_index++;
}

void render_screen(void)
{
    WRITE_LITERAL(clear_screen);
    WRITE_LITERAL(cyan_bold);
    WRITE_LITERAL(top_border);
    WRITE_LITERAL(header);
    WRITE_LITERAL(divider);
    WRITE_LITERAL(reset_color);

    WRITE_LITERAL(cpu_box_top);
    WRITE_LITERAL(green_bold);
    write_matching_line(cpu_buff, cpu_len, "model name", 10);
    WRITE_LITERAL(reset_color);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(mem_box_top);
    WRITE_LITERAL(yellow_bold);
    write_matching_line(mem_buff, mem_len, "MemTotal:", 9);
    write_matching_line(mem_buff, mem_len, "MemAvailable:", 13);
    WRITE_LITERAL(reset_color);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(uptime_box_top);
    write_data_line(uptime_buff, uptime_len);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(dim_color);
    write_status_line();
    WRITE_LITERAL(footer);
    WRITE_LITERAL(bottom_border);
    WRITE_LITERAL(reset_color);
}
