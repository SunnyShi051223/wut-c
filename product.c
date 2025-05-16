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

    char name[50];
    printf("请输入产品名称：");
    fflush(stdout);
    scanf("%49s", name);


    for (int i = 0; i < count; i++) {
        if (strcmp(products[i].name, name) == 0) {
            printf("商品名称 \"%s\" 已存在，禁止重复添加！\n", name);
            return;
        }
    }

    strcpy(products[count].name, name);
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
    load_products();

    int id, qty;
    printf("输入商品ID：");
    fflush(stdout);
    scanf("%d", &id);
    if (id < 1 || id > count) {
        printf("无效商品ID。\n");
        fflush(stdout);
        return;
    }
    printf("输入销售数量：");
    fflush(stdout);
    scanf("%d", &qty);
    if (qty > products[id-1].stock) {
        printf("库存不足！\n");
        fflush(stdout);
        return;
    }
    products[id-1].stock -= qty;
    products[id-1].sold  += qty;

    save_products();
    printf("销售成功。\n");
    fflush(stdout);
}

void restock_product(){
    load_products();

    int id, qty;
    printf("输入商品ID：");
    fflush(stdout);
    scanf("%d", &id);
    if (id < 1 || id > count) {
        printf("无效商品ID。\n");
        fflush(stdout);
        return;
    }
    printf("输入补货数量：");
    fflush(stdout);
    scanf("%d", &qty);
    products[id-1].stock += qty;

    save_products();
    printf("补货成功。\n");
    fflush(stdout);
}
void statistics(){
    int total = 0;
    double revenue = 0;
    load_products();

    for (int i = 0; i < count; i++) {
        total   += products[i].sold;
        revenue += products[i].sold * products[i].price;
    }

    printf("总销售量：%d 件\n", total);
    fflush(stdout);
    printf("总收入：%.2f 元\n", revenue);
    fflush(stdout);
}

void delete_product(){
    load_products();
    int id;
    printf("输入要删除的商品ID：");
    fflush(stdout);
    scanf("%d", &id);
    if (id < 1 || id > count) {
        printf("无效ID。\n");
        fflush(stdout);
        return;
    }
    for (int i = id - 1; i < count - 1; i++) {
        products[i] = products[i + 1];
        products[i].id = i + 1; // 重新编号
    }
    count--;
    save_products();  // 保存到文件
    printf("商品删除成功。\n");
    fflush(stdout);
}

void load_products() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        return;
    }
    count=0;

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
    fclose(fp);
}