//
// Created by 32874 on 2025/5/9.
//

#include "tool.h"
#include <time.h>
#include <string.h>


void format_time(time_t t, char *buf, size_t len) {
    struct tm tm_info;
#if defined(_WIN32)
    localtime_s(&tm_info, &t);
#else
    localtime_r(&t, &tm_info);
#endif
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm_info);
}

char *get_current_time_str(char *buf, size_t len) {
    time_t now = time(NULL);
    format_time(now, buf, len);
    return buf;
}

