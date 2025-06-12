#ifndef DB_H
#define DB_H

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

typedef struct {
    int id;
    char username[50];
    char password[50];
    UserRole role;
} User;

typedef struct {
    int id;
    char name[50];
    float price;
    int stock;
    int user_id;
} Product;

// 数据库连接句柄
extern MYSQL *conn;

// 初始化数据库连接
int db_init();

// 关闭数据库连接
void db_close();

// 用户管理
int db_add_user(const char *username, const char *password, UserRole role);
User db_get_user_by_username(const char *username);
int db_update_user(int user_id, const char *password);
int db_delete_user(int user_id);
int db_list_users();
int db_is_user_logged(int user_id);
int db_set_user_logged(int user_id);
int db_set_user_logged_out(int user_id);

// 商品管理
int db_add_product(const char *name, float price, int stock, int user_id);
Product db_get_product(int product_id,int user_id,int is_admin);
int db_update_product(int product_id, const char *name, float price, int stock, int user_id, int is_admin);
int db_delete_product(int product_id, int user_id, int is_admin);
int db_list_products(int user_id,int is_admin);
int db_search_products(const char *keyword, int user_id, int is_admin);
int db_record_sale(int product_id, int quantity, float unit_price,float total_amount, int user_id);
float db_calculate_sales(int user_id, int is_admin,const char *start_date, const char *end_date);
#endif