//
// Created by 32874 on 2025/5/9.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "product.h"
#include "tool.h"

#define DATA_FILE "data.txt"


static Product products[MAX_PRODUCTS];
static int count = 0;


void add_product(){
    load_products();
    if (count >= MAX_PRODUCTS) {
        printf("产品数量已达到上限，无法添加新产品。\n");
        fflush(stdout);
        return;
    }
    printf("请输入产品名称：");
    fflush(stdout);
    scanf("%s", products[count].name);
    printf("请输入产品价格：");
    fflush(stdout);
    scanf("%lf", &products[count].price);
    printf("请输入产品库存数量：");
    fflush(stdout);
    scanf("%d", &products[count].stock);

    products[count].id = count + 1;
    products[count].sold = 0;
    products[count].added = time(NULL);

    char times[20];
    printf("添加时间：%s\n",
           get_current_time_str(times, sizeof(times)));
    fflush(stdout);
    count++;
    save_products();
    printf("商品添加成功！\n");
    printf("当前内存商品总数：%d\n", count);
    fflush(stdout);
}

void list_product(){
    load_products();

    if (count == 0) {
        printf("当前没有商品。\n");
        fflush(stdout);
        return;
    }

    char times[20];
    printf("ID\t名称\t\t 价格\t  库存\t 已售\t    添加时间\n");
    fflush(stdout);
    for (int i = 0; i < count; i++) {
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

void load_products() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        return;
    }
    count=0;
    printf("正在加载数据！");

    while (fscanf(fp, "%d %s %lf %d %d %ld",
                  &products[count].id,
                  products[count].name,
                  &products[count].price,
                  &products[count].stock,
                  &products[count].sold,
                  &products[count].added) == 6) {
        count++;
        if (count >= MAX_PRODUCTS) break;
    }

    fclose(fp);
}

void save_products() {
    FILE *fp = fopen(DATA_FILE, "w");
    printf("%d",fp==NULL);
    if (!fp) {
        perror("无法打开文件进行保存");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %lf %d %d %ld\n",
                products[i].id,
                products[i].name,
                products[i].price,
                products[i].stock,
                products[i].sold,
                products[i].added);
    }
    printf("正在保存商品到 %s，共 %d 条\n", DATA_FILE, count);
    fclose(fp);
}