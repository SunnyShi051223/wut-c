//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_MODEL_H
#define SYSTEM_VAGETABLE_SALE_MODEL_H

#include "time.h"

#define MAX_PRODUCTS 100
#define NAME_LEN 100

//蔬菜商品结构
typedef struct ProductNode{
    int id; //商品编号
    char name[NAME_LEN];//商品名称
    double price;//商品价格
    int stock;//商品库存
    int sold;//商品已售数量
    time_t added;//商品添加时间
    struct ProductNode* next;
}ProductNode;

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
}MenuChoice;



#endif //SYSTEM_VAGETABLE_SALE_MODEL_H
