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

    db_list_products(currentUser.id, (currentUser.role == ROLE_ADMIN));
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

    Product product = db_get_product(id, currentUser.id, (currentUser.role == ROLE_ADMIN));
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

    printf("请输入新的价格(-1保留原值): ");
    fflush(stdout);
    if (scanf("%f", &new_price) == 1 && new_price >= 0) {
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

    if (db_update_product(id, new_name, new_price, new_stock, currentUser.id, (currentUser.role == ROLE_ADMIN))) {
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

    db_search_products(keyword, currentUser.id, (currentUser.role == ROLE_ADMIN));
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

    Product product = db_get_product(id, currentUser.id, (currentUser.role == ROLE_ADMIN));
    if (product.id == 0) {
        printf("未找到该商品!\n");
        return;
    }

    if (currentUser.role != ROLE_ADMIN && product.user_id != currentUser.id) {
        printf("只能删除自己的商品!\n");
        return;
    }

    if (db_delete_product(id, currentUser.id, (currentUser.role == ROLE_ADMIN))) {
        printf("商品删除成功!\n");
    } else {
        printf("商品删除失败!\n");
    }
}

//销售商品
void sell_product() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    int product_id, quantity;
    printf("输入要销售的商品ID: ");
    fflush(stdout);
    scanf("%d", &product_id);

    Product product = db_get_product(product_id, currentUser.id, (currentUser.role == ROLE_ADMIN));
    if (product.id == 0) {
        printf("未找到该商品或无权操作!\n");
        return;
    }

    printf("当前商品: %s 库存: %d\n", product.name, product.stock);
    printf("输入销售数量: ");
    fflush(stdout);
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("销售数量必须大于0!\n");
        return;
    }

    if (quantity > product.stock) {
        printf("库存不足! 当前库存: %d\n", product.stock);
        return;
    }

    // 记录销售
    float total_amount = product.price * quantity;
    if (db_record_sale(product_id, quantity, product.price, total_amount, currentUser.id)) {
        // 更新库存
        db_update_product(product_id, product.name, product.price,
                          product.stock - quantity, currentUser.id,
                          (currentUser.role == ROLE_ADMIN));
        printf("销售成功! 金额: ¥%.2f\n", total_amount);
    } else {
        printf("销售记录失败!\n");
    }
}

// 统计销售金额
void calculate_sales() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char start_date[20] = {0}, end_date[20] = {0};

    printf("输入开始日期(格式:YYYY-MM-DD, 留空统计所有): ");
    fflush(stdout);
    getchar(); // 清除缓冲区
    if (fgets(start_date, sizeof(start_date), stdin)) {
        start_date[strcspn(start_date, "\n")] = 0; // 移除换行符
    }

    printf("输入结束日期(格式:YYYY-MM-DD, 留空为今日): ");
    fflush(stdout);
    if (fgets(end_date, sizeof(end_date), stdin)) {
        end_date[strcspn(end_date, "\n")] = 0;
    }

    float total = db_calculate_sales(currentUser.id,
                                     (currentUser.role == ROLE_ADMIN),
                                     start_date, end_date);

    if (total < 0) {
        printf("统计失败! 请检查数据库连接和查询\n");
    } else {
        printf("\n=== 销售统计结果 ===\n");
        printf("总金额: ¥%.2f\n", total);

        if (strlen(start_date) > 0 || strlen(end_date) > 0) {
            printf("统计时段: ");
            if (strlen(start_date) > 0) {
                printf("%s", start_date);
            } else {
                printf("所有时间");
            }

            printf(" 至 ");

            if (strlen(end_date) > 0) {
                printf("%s", end_date);
            } else {
                printf("今日");
            }
            printf("\n");
        }
    }
}