//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_PRODUCT_H
#define SYSTEM_VAGETABLE_SALE_PRODUCT_H

#include "model.h"

extern Product products[MAX_PRODUCTS];
extern int count;

void add_product(); // 添加商品
void list_product(); // 商品列表
void sell_product(); // 销售商品
void restock_product();//补货商品
void statistics();//统计
void delete_product();//删除商品


void load_products();
void save_products();

#endif //SYSTEM_VAGETABLE_SALE_PRODUCT_H
