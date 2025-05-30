#include <stdio.h>
#include "product.h"
#include "model.h"
#include "user.h"

int main() {
    int choice;
    do {
        printf("\n===== 助农蔬菜销售系统 =====\n");

        printf("1. 添加商品\n");
        printf("2. 查询商品\n");
        printf("3. 销售商品\n");
        printf("4. 商品补货\n");
        printf("5. 查询统计\n");
        printf("6. 删除商品\n");

        if (current_user.id == 0) {
            printf("7. 用户登录\n");  // 未登录时显示登录选项
        } else {
            printf("7. 用户退出\n");
            if (current_user.is_admin) {
                printf("8. 添加用户\n");  // 管理员显示管理入口
                printf("9. 用户列表\n");
            }
        }

        printf("0. 退出\n");

        printf("请选择操作：");
        fflush(stdout);

        scanf("%d", &choice);

        switch ((MenuChoice)choice) {
            case MENU_ADD_PRODUCT:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    add_product();
                }
                break;
            case MENU_LIST_PRODUCT:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    list_product();
                }
                break;
            case MENU_SALE_PRODUCT:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    sell_product();
                }
                break;
            case MENU_RESTOCK_PRODUCT:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    restock_product();
                }
                break;
            case MENU_STATISTICS:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    statistics();
                }
                break;
            case MENU_DELETE_PRODUCT:
                if (current_user.id == 0) {
                    printf("请先登录系统\n");
                } else {
                    delete_product();
                }
                break;
            case MENU_LOGIN:
                if (current_user.id == 0) {
                    if (user_login()) {
                        printf("登录成功！欢迎 %s\n", current_user.name);
                        load_products();  // 显式加载数据
                    }
                } else {
                    current_user.id = 0;
                    count = 0;  // 退出时清空内存数据
                    printf("已退出登录\n");
                }
                break;
            case MENU_MANAGE_USER:
                if (current_user.is_admin) {
                    add_user();  // 调用用户管理函数
                } else {
                    printf("无权限访问\n");
                }
                break;
            case MENU_LIST_USER:
                if (current_user.is_admin) {
                    show_users();
                }
                else {
                    printf("无权限访问\n");
                }
                break;
            case MENU_EXIT: printf("感谢使用！\n"); break;
            default: printf("无效选择，请重试。\n");
        }
    } while (choice != MENU_EXIT);

    return 0;
}
