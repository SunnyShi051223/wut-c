#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "product.h"

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
        printf("6. 登出\n");  // 新增登出选项
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
            case 6: logout(); return; // 登出并返回
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
        printf("2. 查看我的商品\n");
        printf("3. 搜索商品\n");
        printf("4. 修改商品\n");
        printf("5. 删除商品\n");
        printf("6. 修改密码\n");
        printf("7. 登出\n");  // 新增登出选项
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
            case 7: logout(); return; // 登出并返回
            case 0: return;
            default: printf("无效选择!\n");
        }
    } while (1);
}

int main() {
    printf("正在加载数据...\n");
    load_users();
    load_products();

    // 启动时清除可能存在的登录状态
    FILE *fp = fopen(LOGGED_USERS_FILE, "w");
    if (fp) fclose(fp);

    printf("数据加载完成!\n");

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
                save_users();
                save_products();
                logout();
                printf("感谢使用! 数据已保存\n");
                exit(0);
            default: printf("无效选择!\n");
        }
    } while (1);

    return 0;
}