#include "db.h"
#include "common.h"


MYSQL *conn = NULL;

int db_init() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 0;
    }

    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 0;
    }

    // 设置字符集
    if (mysql_set_character_set(conn, "utf8") != 0) {
        fprintf(stderr, "Failed to set character set: %s\n", mysql_error(conn));
    }

    return 1;
}

void db_close() {
    if (conn) {
        mysql_close(conn);
        conn = NULL;
    }
}

// === 用户管理 ===

int db_add_user(const char *username, const char *password, UserRole role) {
    char query[256];
    sprintf(query, "INSERT INTO users (username, password, role) VALUES ('%s', '%s', '%s')",
            username, password, (role == ROLE_ADMIN) ? "admin" : "user");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Add user failed: %s\n", mysql_error(conn));
        return 0;
    }
    return mysql_insert_id(conn);
}


User db_get_user_by_username(const char *username) {
    User user = {0};
    char query[100];
    sprintf(query, "SELECT * FROM users WHERE username = '%s'", username);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Get user by username failed: %s\n", mysql_error(conn));
        return user;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return user;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        user.id = atoi(row[0]);
        strcpy(user.username, row[1]);
        strcpy(user.password, row[2]);
        user.role = (strcmp(row[3], "admin") == 0) ? ROLE_ADMIN : ROLE_USER;
    }

    mysql_free_result(result);
    return user;
}

int db_update_user(int user_id, const char *password) {
    char query[100];
    sprintf(query, "UPDATE users SET password = '%s' WHERE id = %d", password, user_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Update user failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

int db_delete_user(int user_id) {
    char query[100];
    sprintf(query, "DELETE FROM users WHERE id = %d", user_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Delete user failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

int db_list_users() {
    char query[] = "SELECT * FROM users";

    if (mysql_query(conn, query)) {
        fprintf(stderr, "List users failed: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return 0;
    }

    printf("\n%-5s %-20s %-10s\n", "ID", "用户名", "角色");
    printf("================================\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        printf("%-5s %-20s %s\n", row[0], row[1],
               strcmp(row[3], "admin") == 0 ? "管理员" : "普通用户");
    }

    mysql_free_result(result);
    return 1;
}

int db_is_user_logged(int user_id) {
    char query[100];
    sprintf(query, "SELECT * FROM logged_users WHERE user_id = %d", user_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Check logged user failed: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    int count = result ? mysql_num_rows(result) : 0;
    mysql_free_result(result);

    return count > 0;
}

int db_set_user_logged(int user_id) {
    // 先清除其他登录状态
    char delete_query[100];
    sprintf(delete_query, "DELETE FROM logged_users WHERE user_id = %d", user_id);
    mysql_query(conn, delete_query);

    // 添加新的登录状态
    char insert_query[100];
    sprintf(insert_query, "INSERT INTO logged_users (user_id) VALUES (%d)", user_id);

    if (mysql_query(conn, insert_query)) {
        fprintf(stderr, "Set logged user failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

int db_set_user_logged_out(int user_id) {
    char query[100];
    sprintf(query, "DELETE FROM logged_users WHERE user_id = %d", user_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Set user logged out failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

// === 商品管理 ===

int db_add_product(const char *name, float price, int stock, int user_id) {
    char query[256];
    sprintf(query, "INSERT INTO products (name, price, stock, user_id) VALUES ('%s', %.2f, %d, %d)",
            name, price, stock, user_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Add product failed: %s\n", mysql_error(conn));
        return 0;
    }
    return mysql_insert_id(conn);
}

Product db_get_product(int product_id, int user_id, int is_admin) {
    Product product = {0};
    char query[100];
    sprintf(query, "SELECT * FROM products WHERE id = %d", product_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Get product failed: %s\n", mysql_error(conn));
        return product;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return product;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        product.id = atoi(row[0]);
        strcpy(product.name, row[1]);
        product.price = atof(row[2]);
        product.stock = atoi(row[3]);
        product.user_id = atoi(row[4]);
    }

    mysql_free_result(result);

    // 检查权限：管理员或商品所有者
    if (!is_admin && product.user_id != user_id) {
        product.id = 0; // 表示无权访问
    }

    return product;
}

int db_update_product(int product_id, const char *name, float price, int stock, int user_id, int is_admin) {
    // 先检查商品所属关系
    Product product = db_get_product(product_id, user_id, is_admin);
    if (product.id == 0) {
        printf("您无权修改此商品\n");
        return 0;
    }

    char query[256];
    sprintf(query, "UPDATE products SET name = '%s', price = %.2f, stock = %d WHERE id = %d",
            name, price, stock, product_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Update product failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}
int db_delete_product(int product_id, int user_id, int is_admin) {
    // 先检查商品所属关系
    Product product = db_get_product(product_id, user_id, is_admin);
    if (product.id == 0) {
        printf("您无权删除此商品\n");
        return 0;
    }

    char query[100];
    sprintf(query, "DELETE FROM products WHERE id = %d", product_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Delete product failed: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}

int db_list_products(int user_id, int is_admin) {
    char query[256];
    if (is_admin) {
        strcpy(query, "SELECT p.*, u.username FROM products p JOIN users u ON p.user_id = u.id");
    } else {
        sprintf(query, "SELECT p.*, u.username FROM products p JOIN users u ON p.user_id = u.id WHERE p.user_id = %d", user_id);
    }

    if (mysql_query(conn, query)) {
        fprintf(stderr, "List products failed: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        if (is_admin) {
            printf("系统暂无商品信息\n");
        } else {
            printf("您尚未添加任何商品\n");
        }
        return 0;
    }

    int num_rows = mysql_num_rows(result);

    if (num_rows == 0) {
        if (is_admin) {
            printf("系统暂无商品信息\n");
        } else {
            printf("您尚未添加任何商品\n");
        }
        mysql_free_result(result);
        return 0;
    }

    printf("\n%-5s %-20s %-8s %-6s %-20s\n", "ID", "名称", "价格", "库存", "所有者");
    printf("==========================================================\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        printf("%-5s %-20s %-8s %-6s %-20s\n", row[0], row[1], row[2], row[3], row[5]);
    }

    mysql_free_result(result);
    return 1;
}
int db_search_products(const char *keyword, int user_id, int is_admin) {
    char query[256];
    if (is_admin) {
        sprintf(query, "SELECT p.*, u.username FROM products p JOIN users u ON p.user_id = u.id WHERE p.name LIKE '%%%s%%'", keyword);
    } else {
        sprintf(query, "SELECT p.*, u.username FROM products p JOIN users u ON p.user_id = u.id WHERE p.user_id = %d AND p.name LIKE '%%%s%%'",
                user_id, keyword);
    }

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Search products failed: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        printf("未找到匹配商品\n");
        return 0;
    }

    int num_rows = mysql_num_rows(result);
    if (num_rows == 0) {
        printf("未找到匹配商品\n");
        mysql_free_result(result);
        return 0;
    }

    printf("\n=== 搜索结果 ===\n");
    printf("%-5s %-20s %-8s %-6s %-20s\n", "ID", "名称", "价格", "库存", "所有者");
    printf("==========================================================\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        printf("%-5s %-20s %-8s %-6s %-20s\n", row[0], row[1], row[2], row[3], row[5]);
    }

    mysql_free_result(result);
    return num_rows;
}