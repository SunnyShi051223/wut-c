//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_MODEL_H
#define SYSTEM_VAGETABLE_SALE_MODEL_H

#include "time.h"

#define MAX_PRODUCTS 100
#define NAME_LEN 100
#define MAX_USERS 20  // 新增：最大用户数量

// 用户结构体
typedef struct {
    int id;           // 用户编号
    char name[NAME_LEN];  // 用户名
    char password[NAME_LEN]; // 密码
    int is_admin;     // 是否为管理员（0-普通用户，1-管理员）
    int is_login;  // 登录状态标识 (0-未登录, 1-已登录)
} User;

//蔬菜商品结构
typedef struct {
    int id; 
    int user_id;  // 关联用户ID
    char name[NAME_LEN];
    double price;
    int stock;
    int sold;
    time_t added;
}Product;

//菜单结构-枚举类型
typedef enum{
    // 退出菜单
    MENU_EXIT=0,
    // 添加商品菜单
    MENU_ADD_PRODUCT=1,
    // 列出商品菜单
    MENU_LIST_PRODUCT=2,
    // 销售商品菜单
    MENU_SALE_PRODUCT=3,
    // 补货商品菜单
    MENU_RESTOCK_PRODUCT=4,
    // 统计商品菜单
    MENU_STATISTICS=5,
    // 删除商品菜单
    MENU_DELETE_PRODUCT=6,
    // 新增用户相关菜单
    MENU_LOGIN=7,         // 用户登录
    MENU_MANAGE_USER=8,   // 管理用户（仅管理员可见）
    MENU_LIST_USER=9         // 用户退出
}MenuChoice;



#endif //SYSTEM_VAGETABLE_SALE_MODEL_H
