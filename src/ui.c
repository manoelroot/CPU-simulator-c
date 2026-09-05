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

static long number_to_text(unsigned long value, char *buffer)
{
    long length = 0;
    long index;

    do
    {
        buffer[length++] = (char)('0' + value % 10);
        value /= 10;
    } while (value > 0);

    for (index = 0; index < length / 2; index++)
    {
        char swap = buffer[index];
        buffer[index] = buffer[length - index - 1];
        buffer[length - index - 1] = swap;
    }
    return length;
}

static void write_metric_line(const char *label, long label_len, unsigned long value, const char *suffix, long suffix_len)
{
    char number[24];
    long number_len = number_to_text(value, number);

    WRITE_LITERAL(line_prefix);
    sys_write(STDOUT_FD, label, label_len);
    sys_write(STDOUT_FD, number, number_len);
    sys_write(STDOUT_FD, suffix, suffix_len);
    WRITE_LITERAL(line_suffix);
    WRITE_LITERAL(newline);
}

static void write_load_line(unsigned long load)
{
    char number[24];
    long length;
    char decimal;

    WRITE_LITERAL(line_prefix);
    sys_write(STDOUT_FD, "Load 1 min: ", 12);
    length = number_to_text(load / 100, number);
    sys_write(STDOUT_FD, number, length);
    WRITE_LITERAL(".");
    decimal = (char)('0' + (load % 100) / 10);
    sys_write(STDOUT_FD, &decimal, 1);
    decimal = (char)('0' + load % 10);
    sys_write(STDOUT_FD, &decimal, 1);
    WRITE_LITERAL(line_suffix);
    WRITE_LITERAL(newline);
}

static void write_percent_line(const char *label, long label_len, unsigned long tenths)
{
    char number[24];
    char decimal;
    long length = number_to_text(tenths / 10, number);

    WRITE_LITERAL(line_prefix);
    sys_write(STDOUT_FD, label, label_len);
    sys_write(STDOUT_FD, number, length);
    WRITE_LITERAL(".");
    decimal = (char)('0' + tenths % 10);
    sys_write(STDOUT_FD, &decimal, 1);
    WRITE_LITERAL("%");
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
    const struct system_metrics *system = get_metrics();

    WRITE_LITERAL(clear_screen);
    WRITE_LITERAL(cyan_bold);
    WRITE_LITERAL(top_border);
    WRITE_LITERAL(header);
    WRITE_LITERAL(divider);
    WRITE_LITERAL(reset_color);

    WRITE_LITERAL(cpu_box_top);
    WRITE_LITERAL(green_bold);
    write_matching_line(cpu_buff, cpu_len, "model name", 10);
    write_percent_line("Uso total: ", 11, system->cpu_usage_tenths);
    write_percent_line("User: ", 6, system->cpu_user_tenths);
    write_percent_line("System: ", 8, system->cpu_system_tenths);
    write_load_line(system->load_one_hundredths);
    {
        char number[24];
        long length = number_to_text(system->process_count, number);
        WRITE_LITERAL(line_prefix);
        sys_write(STDOUT_FD, "CPUs: ", 6);
        {
            char cpu_number[24];
            long cpu_length = number_to_text(system->cpu_count, cpu_number);
            sys_write(STDOUT_FD, cpu_number, cpu_length);
        }
        WRITE_LITERAL("  Processos: ");
        sys_write(STDOUT_FD, number, length);
        WRITE_LITERAL(line_suffix);
        WRITE_LITERAL(newline);
    }
    WRITE_LITERAL(reset_color);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(mem_box_top);
    WRITE_LITERAL(yellow_bold);
    write_metric_line("Usada: ", 7, system->memory_used_kb, " kB", 3);
    write_metric_line("Total: ", 7, system->memory_total_kb, " kB", 3);
    write_metric_line("Uso: ", 5, system->memory_percent, "%", 1);
    write_metric_line("Disponivel: ", 12, system->memory_available_kb, " kB", 3);
    write_metric_line("Swap livre: ", 12, system->swap_free_kb, " kB", 3);
    WRITE_LITERAL(reset_color);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(uptime_box_top);
    write_metric_line("Segundos: ", 10, system->uptime_seconds, "", 0);
    WRITE_LITERAL(blank_line);

    WRITE_LITERAL(dim_color);
    write_status_line();
    WRITE_LITERAL(footer);
    WRITE_LITERAL(bottom_border);
    WRITE_LITERAL(reset_color);
}
