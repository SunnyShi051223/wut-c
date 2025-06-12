#ifndef COMMON_H
#define COMMON_H

// 系统配置常量
#define MAX_USERS 50
#define MAX_PRODUCTS 100
#define MAX_NAME_LEN 50
#define USER_FILE "users.txt"
#define PRODUCT_FILE "products.txt"
#define LOGGED_USERS_FILE "logged_users.txt"

// 用户角色枚举
typedef enum {
    ROLE_ADMIN,
    ROLE_USER
} UserRole;

#endif