//
// Created by 32874 on 2025/5/9.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "product.h"
#include "user.h"  // 需要访问当前用户

// 全局商品数据
Product products[MAX_PRODUCTS];
int productCount = 0;

// 添加新商品
void add_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    if (productCount >= MAX_PRODUCTS) {
        printf("商品数量已达上限!\n");
        return;
    }

    Product newProduct;
    printf("商品名称: ");
    scanf("%49s", newProduct.name);

    // 检查商品名称是否重复
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].name, newProduct.name) == 0) {
            printf("商品名称已存在!\n");
            return;
        }
    }

    printf("单价: ");
    scanf("%f", &newProduct.price);
    printf("库存数量: ");
    scanf("%d", &newProduct.stock);

    if (newProduct.price <= 0 || newProduct.stock < 0) {
        printf("价格或库存值不合法!\n");
        return;
    }

    newProduct.id = 1000 + productCount;
    newProduct.userId = currentUser.id;
    products[productCount++] = newProduct;

    save_products();
    printf("添加成功! 商品ID: %d\n", newProduct.id);
}

// 查看商品列表
void list_products() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    printf("\n%-6s %-20s %-10s %-8s %-10s\n",
           "ID", "名称", "单价", "库存", "所有者");
    printf("===================================================\n");

    for (int i = 0; i < productCount; i++) {
        // 管理员可查看所有商品，普通用户只能查看自己的
        if (currentUser.role == ROLE_ADMIN ||
            products[i].userId == currentUser.id) {

            // 查找商品所有者用户名
            char owner[20] = "未知";
            for (int j = 0; j < userCount; j++) {
                if (users[j].id == products[i].userId) {
                    strcpy(owner, users[j].username);
                    break;
                }
            }

            printf("%-6d %-20s %-10.2f %-8d %-10s\n",
                   products[i].id,
                   products[i].name,
                   products[i].price,
                   products[i].stock,
                   owner);
        }
    }

    if (productCount == 0) {
        printf("暂无商品信息\n");
    }
}

// 修改商品信息
void modify_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    int id;
    printf("输入要修改的商品ID: ");
    scanf("%d", &id);

    int found = -1;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            // 检查权限：只能修改自己的商品，或管理员可以修改所有
            if (currentUser.role != ROLE_ADMIN &&
                products[i].userId != currentUser.id) {
                printf("只能修改自己的商品!\n");
                return;
            }
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("未找到该商品!\n");
        return;
    }

    printf("当前商品信息: %s 单价: %.2f 库存: %d\n",
           products[found].name,
           products[found].price,
           products[found].stock);

    printf("请输入新的商品名称(直接回车保留原值): ");
    char new_name[MAX_NAME_LEN];
    getchar();  // 清除输入缓冲区
    if (fgets(new_name, sizeof(new_name), stdin) != NULL) {
        // 移除末尾的换行符
        new_name[strcspn(new_name, "\n")] = 0;

        if (strlen(new_name) > 0) {
            // 检查新名称是否与其他商品重复
            for (int i = 0; i < productCount; i++) {
                if (i != found && strcmp(products[i].name, new_name) == 0) {
                    printf("商品名称已存在!\n");
                    return;
                }
            }
            strcpy(products[found].name, new_name);
        }
    }

    printf("请输入新的价格(0保留原值): ");
    float new_price;
    if (scanf("%f", &new_price) == 1 && new_price > 0) {
        products[found].price = new_price;
    }

    printf("请输入新的库存数量(0保留原值): ");
    int new_stock;
    if (scanf("%d", &new_stock) == 1 && new_stock >= 0) {
        products[found].stock = new_stock;
    }

    save_products();
    printf("商品信息修改成功!\n");
}

// 搜索商品
void search_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char keyword[50];
    printf("请输入搜索关键字: ");
    scanf("%49s", keyword);

    printf("\n=== 搜索结果 ===\n");
    int found = 0;

    for (int i = 0; i < productCount; i++) {
        // 只搜索当前用户的商品
        if (products[i].userId == currentUser.id &&
            strstr(products[i].name, keyword) != NULL) {

            printf("ID:%-6d 名称:%-15s 价格:%-8.2f 库存:%-6d\n",
                   products[i].id,
                   products[i].name,
                   products[i].price,
                   products[i].stock);
            found = 1;
        }
    }

    if (!found) {
        printf("未找到匹配商品\n");
    }
}

// 删除商品
void delete_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    int id;
    printf("输入要删除的商品ID: ");
    scanf("%d", &id);

    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            // 检查权限
            if (currentUser.role != ROLE_ADMIN &&
                products[i].userId != currentUser.id) {
                printf("只能删除自己的商品!\n");
                return;
            }

            // 从商品数组中移除
            for (int j = i; j < productCount - 1; j++) {
                products[j] = products[j + 1];
            }
            productCount--;

            save_products();
            printf("商品删除成功!\n");
            return;
        }
    }

    printf("未找到该商品!\n");
}

// 保存商品数据到文件
void save_products() {
    FILE *fp = fopen(DATA_FILE, "ab");  // 追加模式
    if (!fp) {
        printf("保存商品数据失败!\n");
        return;
    }

    // 保存商品数量和商品数据
    fseek(fp, sizeof(int) + (sizeof(User) * userCount), SEEK_SET);
    fwrite(&productCount, sizeof(int), 1, fp);
    fwrite(products, sizeof(Product), productCount, fp);
    fclose(fp);
}

// 从文件加载商品数据
void load_products() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return;

    // 跳过用户数据部分
    fseek(fp, sizeof(int) + (sizeof(User) * userCount), SEEK_SET);

    fread(&productCount, sizeof(int), 1, fp);
    if (productCount > MAX_PRODUCTS) productCount = MAX_PRODUCTS;
    fread(products, sizeof(Product), productCount, fp);
    fclose(fp);
}