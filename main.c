#include <stdio.h>
#include "product.h"
#include "model.h"

#include <stdio.h>
#include "product.h"
#include "model.h"

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
        printf("0. 退出\n");
        printf("请选择操作：");
        fflush(stdout);
        scanf("%d", &choice);

        switch ((MenuChoice)choice) {
            case MENU_ADD_PRODUCT:    add_product();    break;
            case MENU_LIST_PRODUCT:  list_product();  break;
            case MENU_SALE_PRODUCT:   sell_product();   break;
            case MENU_RESTOCK_PRODUCT:restock_product();break;
            case MENU_STATISTICS:     statistics();     break;
            case MENU_DELETE_PRODUCT: delete_product(); break;
            case MENU_EXIT:           printf("感谢使用！\n"); break;
            default: printf("无效选择，请重试。\n");
        }
    } while (choice != MENU_EXIT);

    return 0;
}
