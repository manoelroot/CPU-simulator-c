#include "sysmon.h"

#define WRITE_LITERAL(value) sys_write(STDOUT_FD, value, sizeof(value) - 1)

static const char clear_screen[] = "\033[2J\033[H\033[?25l";
static const char reset_color[] = "\033[0m";
static const char cyan_bold[] = "\033[1;36m";
static const char green_bold[] = "\033[1;32m";
static const char yellow_bold[] = "\033[1;33m";
static const char dim_color[] = "\033[2m";

static const char top_border[] = "╔══════════════════════════════════════════════════════════════════════════════╗\n";
static const char mid_border[] = "╟──────────────────────────────────────────────────────────────────────────────╢\n";
static const char bottom_border[] = "╚══════════════════════════════════════════════════════════════════════════════╝\n";
static const char empty_line[] = "║                                                                              ║\n";
static const char footer[] = "║   Press q to quit                                                            ║\n";
static const char status_frames[] = "|/-\\";
static unsigned char status_frame_index;

static void write_char(char c) { sys_write(STDOUT_FD, &c, 1); }

static void write_fill(char fill, long count) {
  while (count-- > 0) {
    write_char(fill);
  }
}

static void write_cropped(const char *data, long len, long max_len) {
  if (len > max_len) {
    len = max_len;
  }

  if (len > 0) {
    sys_write(STDOUT_FD, data, len);
  }
}

static long key_matches(const char *line, const char *end, const char *key,
                        long key_len) {
  long index;

  for (index = 0; index < key_len; index++) {
    if (line + index >= end || line[index] != key[index]) {
      return 0;
    }
  }

  return 1;
}

static long parse_number(const char *data, long len) {
  long value = 0;
  long index = 0;

  while (index < len && (data[index] < '0' || data[index] > '9')) {
    index++;
  }

  while (index < len && data[index] >= '0' && data[index] <= '9') {
    value = value * 10 + (data[index] - '0');
    index++;
  }

  return value;
}

static long parse_proc_kb(const char *buffer, long len, const char *key,
                          long key_len) {
  const char *cursor = buffer;
  const char *end = buffer + len;

  while (cursor < end) {
    const char *line_start = cursor;
    const char *line_end = cursor;

    while (line_end < end && *line_end != '\n') {
      line_end++;
    }

    if (key_matches(line_start, end, key, key_len)) {
      return parse_number(line_start + key_len,
                          line_end - (line_start + key_len));
    }

    cursor = line_end;
    if (cursor < end && *cursor == '\n') {
      cursor++;
    }
  }

  return 0;
}

static void write_row(const char *text, long len) {
  const long width = 72;

  WRITE_LITERAL("║ ");
  write_cropped(text, len, width);
  if (len < width) {
    write_fill(' ', width - len);
  }
  WRITE_LITERAL(" ║\n");
}

static void append_char(char *buffer, long *len, char c) {
  buffer[(*len)++] = c;
}

static void append_literal(char *buffer, long *len, const char *text) {
  long index = 0;
  while (text[index] != '\0') {
    buffer[(*len)++] = text[index++];
  }
}

static void append_decimal_buf(char *buffer, long *len, long value) {
  char temp[24];
  long temp_len = 0;

  if (value == 0) {
    append_char(buffer, len, '0');
    return;
  }

  if (value < 0) {
    append_char(buffer, len, '-');
    value = -value;
  }

  while (value > 0) {
    temp[temp_len++] = (char)('0' + (value % 10));
    value /= 10;
  }

  while (temp_len-- > 0) {
    append_char(buffer, len, temp[temp_len]);
  }
}

static void append_human_kb_buf(char *buffer, long *len, long kb) {
  if (kb >= 1048576L) {
    long whole = kb / 1048576L;
    long fraction = (kb % 1048576L) * 10 / 1048576L;
    append_decimal_buf(buffer, len, whole);
    append_char(buffer, len, '.');
    append_decimal_buf(buffer, len, fraction);
    append_literal(buffer, len, "GiB");
    return;
  }

  if (kb >= 1024L) {
    long whole = kb / 1024L;
    long fraction = (kb % 1024L) * 10 / 1024L;
    append_decimal_buf(buffer, len, whole);
    append_char(buffer, len, '.');
    append_decimal_buf(buffer, len, fraction);
    append_literal(buffer, len, "MiB");
    return;
  }

  append_decimal_buf(buffer, len, kb);
  append_literal(buffer, len, "kB");
}

static void write_memory_line(void) {
  char buffer[96];
  long len = 0;
  long total = parse_proc_kb(mem_buff, mem_len, "MemTotal:", 9);
  long avail = parse_proc_kb(mem_buff, mem_len, "MemAvailable:", 13);
  long used = total - avail;
  long percent = 0;
  long bar_size = 26;
  long filled = 0;

  if (total > 0) {
    percent = (used * 100) / total;
    filled = (used * bar_size) / total;
  }

  append_literal(buffer, &len, "Memory [");
  {
    long index;
    for (index = 0; index < filled; index++) {
      append_char(buffer, &len, '=');
    }
    for (index = filled; index < bar_size; index++) {
      append_char(buffer, &len, ' ');
    }
  }
  append_char(buffer, &len, ']');
  append_literal(buffer, &len, " ");
  append_decimal_buf(buffer, &len, percent);
  append_literal(buffer, &len, "% ");
  append_human_kb_buf(buffer, &len, used);
  append_literal(buffer, &len, " / ");
  append_human_kb_buf(buffer, &len, total);
  write_row(buffer, len);
}

static void write_uptime_line(void) {
  char buffer[64];
  long len = 0;
  long seconds = parse_number(uptime_buff, uptime_len);
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long secs = seconds % 60;

  append_literal(buffer, &len, "Uptime: ");
  append_decimal_buf(buffer, &len, hours);
  append_char(buffer, &len, ':');
  if (minutes < 10) {
    append_char(buffer, &len, '0');
  }
  append_decimal_buf(buffer, &len, minutes);
  append_char(buffer, &len, ':');
  if (secs < 10) {
    append_char(buffer, &len, '0');
  }
  append_decimal_buf(buffer, &len, secs);
  write_row(buffer, len);
}

static void write_matching_line(const char *buffer, long len, const char *key,
                                long key_len) {
  const char *cursor = buffer;
  const char *end = buffer + len;

  while (cursor < end) {
    const char *line_start = cursor;
    const char *line_end = cursor;

    while (line_end < end && *line_end != '\n') {
      line_end++;
    }

    if (key_matches(line_start, end, key, key_len)) {
      write_row(line_start, line_end - line_start);
      return;
    }

    cursor = line_end;
    if (cursor < end && *cursor == '\n') {
      cursor++;
    }
  }
}

static void write_status_line(void) {
  char buffer[88];
  long len = 0;
  char frame = status_frames[status_frame_index & 3];

  append_literal(buffer, &len, "Refresh [");
  append_char(buffer, &len, frame);
  append_literal(buffer, &len,"] Modern C dashboard + Assembly syscalls");
  write_row(buffer, len);
  status_frame_index++;
}

void render_screen(void) {
  WRITE_LITERAL(clear_screen);
  WRITE_LITERAL(cyan_bold);
  WRITE_LITERAL(top_border);
  WRITE_LITERAL("║   ASM-SysMon v0.4 — Modern terminal dashboard                         ║\n");
  WRITE_LITERAL(mid_border);
  WRITE_LITERAL(reset_color);
  WRITE_LITERAL(empty_line);

  WRITE_LITERAL(green_bold);
  write_matching_line(cpu_buff, cpu_len, "model name", 10);
  WRITE_LITERAL(reset_color);
  WRITE_LITERAL(empty_line);

  WRITE_LITERAL(yellow_bold);
  write_memory_line();
  WRITE_LITERAL(reset_color);
  WRITE_LITERAL(empty_line);

  write_uptime_line();
  WRITE_LITERAL(empty_line);

  WRITE_LITERAL(dim_color);
  write_status_line();
  WRITE_LITERAL(footer);
  WRITE_LITERAL(bottom_border);
  WRITE_LITERAL(reset_color);
}
