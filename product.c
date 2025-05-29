//
// Created by 32874 on 2025/5/9.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "product.h"
#include "tool.h"
#include "user.h"

#define DATA_FILE "data.txt"


#define MAX_PRODUCTS 100

Product products[MAX_PRODUCTS];
int count = 0;  // Remove static modifier



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

// 修改后的加载函数
// 新增：当前用户商品数据文件路径
static char product_file[50] = {0};

// 修改商品加载逻辑
// 修改加载函数逻辑
void load_products() {
    if (get_current_user().id == 0) {
        count = 0;  // 清空内存数据
        return;
    }
    
    char filename[50];
    get_user_data_file(filename, sizeof(filename));  // 使用统一路径生成方式
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        count = 0;  // 文件不存在时初始化空列表
        return;
    }
    
    count = 0;  // 每次加载前重置计数器
    while (fscanf(fp, "%d %s %lf %d %d %ld %d",
                  &products[count].id,
                  products[count].name,
                  &products[count].price,
                  &products[count].stock,
                  &products[count].sold,
                  &products[count].added,
                  &products[count].user_id) == 7) {  // 增加user_id字段读取
        count++;
        if (count >= MAX_PRODUCTS) break;
    }
    fclose(fp);
}

// 修改后的保存函数 
void save_products() {
    char filename[50];
    get_user_data_file(filename, sizeof(filename));
    
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("无法打开文件进行保存");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %lf %d %d %ld %d\n",  // 增加user_id字段
                products[i].id,
                products[i].name,
                products[i].price,
                products[i].stock,
                products[i].sold,
                products[i].added,
                products[i].user_id);
    }
    fclose(fp);
}

// 修改添加商品逻辑
void add_product() {
    if (get_current_user().id == 0) {  // 添加双重验证
        printf("错误：请先登录系统\n");
        return;
    }
    
    Product p;
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
    products[count].user_id = get_current_user().id;  // 确保关联用户ID
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
    printf("用户[%s]添加商品成功！\n", get_current_user().name);  // 添加用户信息反馈
}

// 修改商品展示逻辑（仅显示当前用户商品）
void list_product() {
    printf("\n=== 当前用户商品清单 ===\n");
    for (int i = 0; i < count; i++) {
        if (products[i].user_id == get_current_user().id) {
            printf("ID:%d 名称:%-15s 价格:%-6.2f 库存:%d\n", 
                 products[i].id, 
                 products[i].name,
                 products[i].price,  // 新增价格显示
                 products[i].stock);
        }
    }
}

