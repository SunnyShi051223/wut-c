//
// Created by 32874 on 2025/5/9.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "product.h"
#include "tool.h"

static Product products[MAX_PRODUCTS];
static int product_count = 0;


void add_product(){
    if (product_count >= MAX_PRODUCTS) {
        printf("产品数量已达到上限，无法添加新产品。\n");
        fflush(stdout);
        return;
    }
    printf("请输入产品名称：");
    fflush(stdout);
    scanf("%s", products[product_count].name);
    printf("请输入产品价格：");
    fflush(stdout);
    scanf("%lf", &products[product_count].price);
    printf("请输入产品库存数量：");
    fflush(stdout);
    scanf("%d", &products[product_count].stock);

    products[product_count].id = product_count + 1;
    products[product_count].sold = 0;
    products[product_count].added = time(NULL);

    char times[20];
    printf("添加时间：%s\n",
           get_current_time_str(times, sizeof(times)));
    fflush(stdout);
    product_count++;
    printf("商品添加成功！\n");
    fflush(stdout);
}

void list_product(){
    if (product_count == 0) {
        printf("当前没有商品。\n");
        fflush(stdout);
        return;
    }

    char times[20];
    printf("ID\t名称\t\t 价格\t  库存\t 已售\t    添加时间\n");
    fflush(stdout);
    for (int i = 0; i < product_count; i++) {
        format_time(products[i].added, times, sizeof(times));
        printf("%-3d %-8s %-8.2f %-6d %-6d %s\n",
               products[i].id,
               products[i].name,
               products[i].price,
               products[i].stock,
               products[i].sold,
               times);

    }
    fflush(stdout);
}

void sell_product(){

}

void restock_product(){

}
void statistics(){

}

void delete_product(){

}