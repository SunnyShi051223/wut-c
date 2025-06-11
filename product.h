#ifndef PRODUCT_H
#define PRODUCT_H

#include "common.h"

// 商品结构体
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    float price;
    int stock;
    int userId;
} Product;

extern Product products[];
extern int productCount;

void add_product();
void list_products();
void modify_product();
void search_product();
void save_products();
void load_products();
void delete_product();

#endif