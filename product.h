//
// Created by 32874 on 2025/5/9.
//

#ifndef SYSTEM_VAGETABLE_SALE_PRODUCT_H
#define SYSTEM_VAGETABLE_SALE_PRODUCT_H

#include "model.h"

void add_product(); // 添加
void list_product(); // 列表
void sell_product(); // 销售
void restock_product(); // 补货
void statistics(); // 统计
void delete_product(); // 删除

int get_next_id();
ProductNode *create_product_node(int id, const char *name, double price, int stock, int sold, time_t added);


void load_products();
void save_products();

#endif // SYSTEM_VAGETABLE_SALE_PRODUCT_H
