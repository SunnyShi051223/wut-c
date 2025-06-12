#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "user.h"

// 添加商品
void add_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char name[50];
    float price;
    int stock;

    printf("商品名称: ");
    fflush(stdout);
    scanf("%49s", name);

    printf("单价: ");
    fflush(stdout);
    scanf("%f", &price);

    printf("库存数量: ");
    fflush(stdout);
    scanf("%d", &stock);

    if (price <= 0 || stock < 0) {
        printf("价格或库存值不合法!\n");
        return;
    }

    int product_id = db_add_product(name, price, stock, currentUser.id);
    if (product_id > 0) {
        printf("添加成功! 商品ID: %d\n", product_id);
    } else {
        printf("添加失败!\n");
    }
}

// 商品列表
void list_products() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    if (!db_list_products()) {
        printf("获取商品列表失败\n");
    }
}

// 修改商品
void modify_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    int id;
    printf("输入要修改的商品ID: ");
    fflush(stdout);
    scanf("%d", &id);

    Product product = db_get_product(id);
    if (product.id == 0) {
        printf("未找到该商品!\n");
        return;
    }

    if (currentUser.role != ROLE_ADMIN && product.user_id != currentUser.id) {
        printf("只能修改自己的商品!\n");
        return;
    }

    printf("当前商品信息: %s 单价: %.2f 库存: %d\n",
           product.name, product.price, product.stock);

    char new_name[50] = {0};
    float new_price = 0;
    int new_stock = -1;

    printf("请输入新的商品名称(直接回车保留原值): ");
    fflush(stdout);
    getchar(); // 消耗换行符
    if (fgets(new_name, sizeof(new_name), stdin) != NULL && new_name[0] != '\n') {
        // 移除换行符
        new_name[strcspn(new_name, "\n")] = 0;
    } else {
        // 用户没有输入新名称，保持原值
        strcpy(new_name, product.name);
    }

    printf("请输入新的价格(0保留原值): ");
    fflush(stdout);
    if (scanf("%f", &new_price) == 1 && new_price > 0) {
        // 用户输入了有效的新价格
    } else {
        // 保留原价格
        new_price = product.price;
    }

    printf("请输入新的库存数量(-1保留原值): ");
    fflush(stdout);
    if (scanf("%d", &new_stock) == 1 && new_stock >= 0) {
        // 用户输入了有效的库存
    } else {
        // 保留原库存
        new_stock = product.stock;
    }

    if (db_update_product(id, new_name, new_price, new_stock)) {
        printf("商品信息修改成功!\n");
    } else {
        printf("商品信息修改失败!\n");
    }
}

// 搜索商品
void search_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char keyword[50];
    printf("请输入搜索关键字: ");
    fflush(stdout);
    scanf("%49s", keyword);

    db_search_products(keyword, currentUser.id);
}

// 删除商品
void delete_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    int id;
    printf("输入要删除的商品ID: ");
    fflush(stdout);
    scanf("%d", &id);

    Product product = db_get_product(id);
    if (product.id == 0) {
        printf("未找到该商品!\n");
        return;
    }

    if (currentUser.role != ROLE_ADMIN && product.user_id != currentUser.id) {
        printf("只能删除自己的商品!\n");
        return;
    }

    if (db_delete_product(id)) {
        printf("商品删除成功!\n");
    } else {
        printf("商品删除失败!\n");
    }
}