#include "sysmon.h"

struct cpu_snapshot
{
        unsigned long user;
        unsigned long nice;
        unsigned long system;
        unsigned long idle;
        unsigned long iowait;
        unsigned long irq;
        unsigned long softirq;
        unsigned long steal;
};

static struct cpu_snapshot previous_cpu;
static struct system_metrics metrics;

static long is_digit(char value)
{
        return value >= '0' && value <= '9';
}

static unsigned long parse_number(const char **cursor, const char *end)
{
        unsigned long value = 0;

        while (*cursor < end && is_digit(**cursor))
        {
                value = value * 10 + (unsigned long)(**cursor - '0');
                (*cursor)++;
        }
        return value;
}

static const char *skip_to_line(const char *cursor, const char *end)
{
        while (cursor < end && *cursor != '\n')
        {
                cursor++;
        }
        return cursor;
}

static void parse_cpu_stats(void)
{
        const char *cursor = stat_buff;
        const char *end = stat_buff + stat_len;
        struct cpu_snapshot current;
        unsigned long total_delta;
        unsigned long idle_delta;
        current.user = 0;
        current.nice = 0;
        current.system = 0;
        current.idle = 0;
        current.iowait = 0;
        current.irq = 0;
        current.softirq = 0;
        current.steal = 0;
        metrics.cpu_count = 1;

        if (cursor + 4 >= end || cursor[0] != 'c' || cursor[1] != 'p' || cursor[2] != 'u' || cursor[3] != ' ')
        {
                return;
        }
        cursor += 4;
        current.user = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.nice = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.system = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.idle = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.iowait = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.irq = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.softirq = parse_number(&cursor, end);
        while (cursor < end && *cursor == ' ')
                cursor++;
        current.steal = parse_number(&cursor, end);

        total_delta = current.user + current.nice + current.system + current.idle + current.iowait + current.irq + current.softirq + current.steal;
        total_delta -= previous_cpu.user + previous_cpu.nice + previous_cpu.system + previous_cpu.idle + previous_cpu.iowait + previous_cpu.irq + previous_cpu.softirq + previous_cpu.steal;
        idle_delta = (current.idle + current.iowait) - (previous_cpu.idle + previous_cpu.iowait);

        if (metrics.sample_count > 0 && total_delta > 0)
        {
                metrics.cpu_usage_tenths = ((total_delta - idle_delta) * 1000) / total_delta;
                metrics.cpu_user_tenths = ((current.user + current.nice - previous_cpu.user - previous_cpu.nice) * 1000) / total_delta;
                metrics.cpu_system_tenths = ((current.system - previous_cpu.system) * 1000) / total_delta;
                metrics.cpu_iowait_tenths = ((current.iowait - previous_cpu.iowait) * 1000) / total_delta;
        }
        previous_cpu = current;

        cursor = skip_to_line(stat_buff, end);
        while (cursor < end)
        {
                cursor++;
                if (cursor + 4 < end && cursor[0] == 'c' && cursor[1] == 'p' && cursor[2] == 'u' && is_digit(cursor[3]))
                {
                        metrics.cpu_count++;
                }
                else if (cursor + 10 < end && cursor[0] == 'p' && cursor[1] == 'r' && cursor[2] == 'o' && cursor[3] == 'c' && cursor[4] == 'e' && cursor[5] == 's' && cursor[6] == 's' && cursor[7] == 'e' && cursor[8] == 's' && cursor[9] == ' ')
                {
                        cursor += 10;
                        metrics.process_count = parse_number(&cursor, end);
                }
                cursor = skip_to_line(cursor, end);
        }
}

static void parse_memory(void)
{
        const char *cursor = mem_buff;
        const char *end = mem_buff + mem_len;
        unsigned long total = 0;
        unsigned long available = 0;
        unsigned long swap_total = 0;
        unsigned long swap_free = 0;

        while (cursor < end)
        {
                if (cursor + 9 < end && cursor[0] == 'M' && cursor[1] == 'e' && cursor[2] == 'm' && cursor[3] == 'T' && cursor[4] == 'o' && cursor[5] == 't' && cursor[6] == 'a' && cursor[7] == 'l' && cursor[8] == ':')
                {
                        cursor += 9;
                        while (cursor < end && !is_digit(*cursor))
                                cursor++;
                        total = parse_number(&cursor, end);
                }
                else if (cursor + 13 < end && cursor[0] == 'M' && cursor[1] == 'e' && cursor[2] == 'm' && cursor[3] == 'A' && cursor[4] == 'v' && cursor[5] == 'a' && cursor[6] == 'i' && cursor[7] == 'l' && cursor[8] == 'a' && cursor[9] == 'b' && cursor[10] == 'l' && cursor[11] == 'e' && cursor[12] == ':')
                {
                        cursor += 13;
                        while (cursor < end && !is_digit(*cursor))
                                cursor++;
                        available = parse_number(&cursor, end);
                }
                else if (cursor + 10 < end && cursor[0] == 'S' && cursor[1] == 'w' && cursor[2] == 'a' && cursor[3] == 'p' && cursor[4] == 'T' && cursor[5] == 'o' && cursor[6] == 't' && cursor[7] == 'a' && cursor[8] == 'l' && cursor[9] == ':')
                {
                        cursor += 10;
                        while (cursor < end && !is_digit(*cursor))
                                cursor++;
                        swap_total = parse_number(&cursor, end);
                }
                else if (cursor + 9 < end && cursor[0] == 'S' && cursor[1] == 'w' && cursor[2] == 'a' && cursor[3] == 'p' && cursor[4] == 'F' && cursor[5] == 'r' && cursor[6] == 'e' && cursor[7] == 'e' && cursor[8] == ':')
                {
                        cursor += 9;
                        while (cursor < end && !is_digit(*cursor))
                                cursor++;
                        swap_free = parse_number(&cursor, end);
                }
                cursor = skip_to_line(cursor, end);
                if (cursor < end)
                        cursor++;
        }

        metrics.memory_total_kb = total;
        metrics.memory_available_kb = available;
        metrics.memory_used_kb = total > available ? total - available : 0;
        metrics.memory_percent = total > 0 ? (metrics.memory_used_kb * 100) / total : 0;
        metrics.swap_total_kb = swap_total;
        metrics.swap_free_kb = swap_free;
}

static void parse_uptime(void)
{
        const char *cursor = uptime_buff;
        const char *end = uptime_buff + uptime_len;
        metrics.uptime_seconds = parse_number(&cursor, end);
}

static void parse_load(void)
{
        const char *cursor = load_buff;
        const char *end = load_buff + load_len;
        unsigned long whole = parse_number(&cursor, end);
        unsigned long fraction = 0;

        if (cursor < end && *cursor == '.')
        {
                cursor++;
                fraction = parse_number(&cursor, end);
                if (fraction < 10)
                        fraction *= 10;
        }
        metrics.load_one_hundredths = whole * 100 + fraction;

        while (cursor < end && *cursor == ' ')
                cursor++;
        parse_number(&cursor, end);
        if (cursor < end && *cursor == '.')
        {
                cursor++;
                parse_number(&cursor, end);
        }
        while (cursor < end && *cursor == ' ')
                cursor++;
        parse_number(&cursor, end);
        if (cursor < end && *cursor == '.')
        {
                cursor++;
                parse_number(&cursor, end);
        }
        while (cursor < end && *cursor == ' ')
                cursor++;
        parse_number(&cursor, end);
        if (cursor < end && *cursor == '/')
        {
                cursor++;
                metrics.process_count = parse_number(&cursor, end);
        }
}

void update_metrics(void)
{
        parse_cpu_stats();
        parse_memory();
        parse_uptime();
        parse_load();
        metrics.sample_count++;
}

const struct system_metrics *get_metrics(void)
{
        return &metrics;
}
