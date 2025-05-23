//
// Created by 32874 on 2025/5/9.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "product.h"
#include "tool.h"

#define DATA_FILE "data.txt"


static ProductNode *head=NULL;

ProductNode *create_product_node(int id, const char *name, double price, int stock, int sold, time_t added) {
    ProductNode *new_node = (ProductNode *)malloc(sizeof(ProductNode));
    if (!new_node) {
        perror("内存分配失败");
        exit(1);
    }
    new_node->id = id;
    strncpy(new_node->name, name, NAME_LEN - 1);
    new_node->name[NAME_LEN - 1] = '\0';
    new_node->price = price;
    new_node->stock = stock;
    new_node->sold = sold;
    new_node->added = added;
    new_node->next = NULL;
    return new_node;
}

int get_next_id() {
    int max_id = 0;
    ProductNode *current = head;
    while (current) {
        if (current->id > max_id) {
            max_id = current->id;
        }
        current = current->next;
    }
    return max_id + 1;
}


void add_product() {
    char name[50];
    printf("请输入产品名称：");
    fflush(stdout);
    scanf("%49s", name);

    load_products();

    ProductNode *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            printf("商品名称 \"%s\" 已存在，禁止重复添加！\n", name);
            return;
        }
        current = current->next;
    }

    double price;
    int stock;
    printf("请输入产品价格：");
    fflush(stdout);
    scanf("%lf", &price);
    printf("请输入产品库存数量：");
    fflush(stdout);
    scanf("%d", &stock);

    time_t now = time(NULL);
    char times[20];
    printf("添加时间：%s\n", get_current_time_str(times, sizeof(times)));

    ProductNode *new_node = create_product_node(get_next_id(), name, price, stock, 0, now);
    new_node->next = head;
    head = new_node;

    save_products();
    printf("商品添加成功！\n");
}




void list_product() {

    load_products();
    if (!head) {
        printf("当前没有商品。\n");
        fflush(stdout);
        return;
    }

    char times[20];
    printf("ID\t名称\t\t 价格\t  库存\t 已售\t    添加时间\n");
    fflush(stdout);

    ProductNode *current = head;
    while (current) {
        format_time(current->added, times, sizeof(times));
        printf("%-3d %-8s %-8.2f %-6d %-6d %s\n",
               current->id,
               current->name,
               current->price,
               current->stock,
               current->sold,
               times);
        current = current->next;
    }
}

void sell_product() {
    int id, qty;
    printf("输入商品ID：");
    fflush(stdout);
    scanf("%d", &id);

    load_products();

    ProductNode *current = head;

    while (current) {
        if (current->id == id) {
            printf("输入销售数量：");
            fflush(stdout);
            scanf("%d", &qty);
            if (qty > current->stock) {
                printf("库存不足！\n");
                fflush(stdout);
                return;
            }
            current->stock -= qty;
            current->sold += qty;
            save_products();
            printf("销售成功。\n");
            fflush(stdout);
            return;
        }
        current = current->next;
    }

    printf("无效商品ID。\n");
    fflush(stdout);
}

void restock_product() {
    int id, qty;
    printf("输入商品ID：");
    fflush(stdout);
    scanf("%d", &id);

    load_products();

    ProductNode *current = head;
    while (current) {
        if (current->id == id) {
            printf("输入补货数量：");
            fflush(stdout);
            scanf("%d", &qty);
            current->stock += qty;
            save_products();
            printf("补货成功。\n");
            fflush(stdout);
            return;
        }
        current = current->next;
    }

    printf("无效商品ID。\n");
    fflush(stdout);
}

void statistics() {
    int total = 0;
    double revenue = 0;
    load_products();

    ProductNode *current = head;
    while (current) {
        total += current->sold;
        revenue += current->sold * current->price;
        current = current->next;
    }

    printf("总销售量：%d 件\n", total);
    fflush(stdout);
    printf("总收入：%.2f 元\n", revenue);
    fflush(stdout);
}

void delete_product() {
    load_products();
    int id;
    printf("输入要删除的商品ID：");
    fflush(stdout);
    scanf("%d", &id);
    while (getchar() != '\n');

    ProductNode *current = head;
    ProductNode *prev = NULL;

    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            free(current);
            save_products();
            printf("商品删除成功。\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("无效ID。\n");
}

void load_products() {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) return;

    while (head) {
        ProductNode *temp = head;
        head = head->next;
        free(temp);
    }

    int id;
    char name[NAME_LEN];
    double price;
    int stock, sold;
    long added;

    while (fscanf(fp, "%d %99s %lf %d %d %ld", &id, name, &price, &stock, &sold, &added) == 6) {
        ProductNode *new_node = create_product_node(id, name, price, stock, sold, (time_t)added);
        new_node->next = head;
        head = new_node;
    }

    fclose(fp);
}

void save_products() {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) {
        perror("无法打开文件进行保存");
        return;
    }

    ProductNode *current = head;
    while (current) {
        fprintf(fp, "%d %s %lf %d %d %ld\n",
                current->id,
                current->name,
                current->price,
                current->stock,
                current->sold,
                current->added);
        current = current->next;
    }
    fclose(fp);
}