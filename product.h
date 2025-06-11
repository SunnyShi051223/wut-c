//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_PRODUCT_H
#define SYSTEM_VAGETABLE_SALE_PRODUCT_H


#include "common.h"

// 商品结构体
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    int stock;
    int userId;  // 所属用户ID
} Product;

// 全局商品数据
extern Product products[];
extern int productCount;

// 商品管理函数
void add_product();
void list_products();
void modify_product();
void search_product();
void save_products();
void load_products();
void delete_product();


#endif //SYSTEM_VAGETABLE_SALE_PRODUCT_H
