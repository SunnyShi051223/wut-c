#ifndef COMMON_H
#define COMMON_H

// MySQL 数据库配置
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWORD "shisannian1223"
#define DB_NAME "vagetable"

// 系统配置常量
#define MAX_NAME_LEN 50

// 用户角色枚举
typedef enum {
    ROLE_ADMIN,
    ROLE_USER
} UserRole;

#endif