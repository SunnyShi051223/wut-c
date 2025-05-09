//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_TOOL_H
#define SYSTEM_VAGETABLE_SALE_TOOL_H

#ifndef TOOL_H
#define TOOL_H

#include <time.h>
#include <stddef.h>



// 格式化时间函数，将时间t格式化为字符串，存储在buf中，长度为len
void format_time(time_t t, char *buf, size_t len);

// 获取当前时间字符串
char *get_current_time_str(char *buf, size_t len);

#endif /* TOOL_H */


#endif //SYSTEM_VAGETABLE_SALE_TOOL_H
