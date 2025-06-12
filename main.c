#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "product.h"
#include "db.h"

// 管理员菜单
void admin_menu() {
    int choice;
    do {
        printf("\n==== 管理员菜单 ====\n");
        printf("1. 查看所有商品\n");
        printf("2. 用户列表\n");
        printf("3. 添加用户\n");
        printf("4. 删除用户\n");
        printf("5. 修改密码\n");
        printf("6. 登出\n");
        printf("0. 返回主菜单\n");
        printf("====================\n选择操作: ");
        fflush(stdout);
        scanf("%d", &choice);

        switch (choice) {
            case 1: list_products(); break;
            case 2: list_users(); break;
            case 3: register_user(); break;
            case 4: delete_user(); break;
            case 5: change_password(); break;
            case 6: logout(); return;
            case 0: return;
            default: printf("无效选择!\n");
        }
    } while (1);
}

// 普通用户菜单
void user_menu() {
    int choice;
    do {
        printf("\n==== 用户菜单: %s ====\n", currentUser.username);
        printf("1. 添加商品\n");
        printf("2. 查看所有商品\n");
        printf("3. 搜索商品\n");
        printf("4. 修改商品\n");
        printf("5. 删除商品\n");
        printf("6. 修改密码\n");
        printf("7. 销售商品\n");          // 新增选项
        printf("8. 统计销售金额\n");        // 新增选项
        printf("9. 登出\n");
        printf("0. 返回主菜单\n");
        printf("=======================\n选择操作: ");
        fflush(stdout);
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_product(); break;
            case 2: list_products(); break;
            case 3: search_product(); break;
            case 4: modify_product(); break;
            case 5: delete_product(); break;
            case 6: change_password(); break;
            case 7: sell_product(); break;     // 调用销售功能
            case 8: calculate_sales(); break;   // 调用统计功能
            case 9: logout(); return;
            case 0: return;
            default: printf("无效选择!\n");
        }
    } while (1);
}

int main() {
    // 初始化数据库连接
    if (!db_init()) {
        fprintf(stderr, "数据库连接失败!\n");
        return 1;
    }
    printf("数据库连接成功!\n");

    // 确保管理员账户存在
    add_admin_if_empty();

    int main_choice;
    do {
        printf("\n======== 助农蔬菜销售系统 ========\n");
        if (currentUser.id) {
            printf("当前用户: %s [%s]\n",
                   currentUser.username,
                   currentUser.role == ROLE_ADMIN ? "管理员" : "普通用户");
        } else {
            printf("当前未登录\n");
        }
        printf("1. 注册\n");
        printf("2. 登录\n");
        printf("3. 退出系统\n");
        printf("=================================\n选择操作: ");
        fflush(stdout);
        scanf("%d", &main_choice);

        switch (main_choice) {
            case 1:
                register_user();
                break;
            case 2:
                if (login()) {
                    if (currentUser.role == ROLE_ADMIN) {
                        admin_menu();
                    } else {
                        user_menu();
                    }
                }
                break;
            case 3:
                logout();
                db_close();
                printf("感谢使用!\n");
                exit(0);
            default: printf("无效选择!\n");
        }
    } while (1);

    db_close();
    return 0;
}