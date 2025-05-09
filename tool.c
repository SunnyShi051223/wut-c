//
// Created by 32874 on 2025/5/9.
//

#include "tool.h"
#include <time.h>
#include <string.h>


// 格式化时间函数
void format_time(time_t t, char *buf, size_t len) {
    struct tm tm_info;
    if (localtime_s(&tm_info, &t) != 0) {
        strncpy(buf, "未知时间", len);
        buf[len - 1] = '\0';
        return;
    }
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm_info);
}

// 获取当前时间字符串
char *get_current_time_str(char *buf, size_t len) {
    time_t now = time(NULL);
    format_time(now, buf, len);
    return buf;
}

