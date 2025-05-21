//
// Created by 32874 on 2025/5/21.
//

#include "window.h"
#include "product.h"
#include "tool.h"
#include <gtk/gtk.h>
#include <string.h>


// 声明全局控件
GtkWidget *product_list;
GtkWidget *status_label;

// 刷新商品列表
void refresh_product_list(GtkWidget *list) {
    GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
    gtk_list_store_clear(store);

    for (int i = 0; i < count; i++) {
        char time_str[20];
        char price_str[20];
        format_time(products[i].added, time_str, sizeof(time_str));
        snprintf(price_str, sizeof(price_str), "%.2f", products[i].price);

        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, products[i].id,
                           1, products[i].name,
                           2, price_str,
                           3, products[i].stock,
                           4, products[i].sold,
                           5, time_str,
                           -1);
    }
}
// 添加商品对话框
void on_add_product(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("添加商品", GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "确定", GTK_RESPONSE_ACCEPT,
                                                    "取消", GTK_RESPONSE_REJECT,
                                                    NULL);

    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *price_entry = gtk_entry_new();
    GtkWidget *stock_entry = gtk_entry_new();

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("名称:"));
    gtk_container_add(GTK_CONTAINER(content), name_entry);
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("价格:"));
    gtk_container_add(GTK_CONTAINER(content), price_entry);
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("库存:"));
    gtk_container_add(GTK_CONTAINER(content), stock_entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        double price = atof(gtk_entry_get_text(GTK_ENTRY(price_entry)));
        int stock = atoi(gtk_entry_get_text(GTK_ENTRY(stock_entry)));

        // 调用原有业务逻辑
        // (需要稍改 add_product 函数，此处简化为直接操作)
        strcpy(products[count].name, name);
        products[count].price = price;
        products[count].stock = stock;
        products[count].id = count + 1;
        products[count].sold = 0;
        products[count].added = time(NULL);
        count++;

        save_products();

        gtk_label_set_text(GTK_LABEL(status_label), "商品添加成功！");
        refresh_product_list(product_list);
    }
    gtk_widget_destroy(dialog);
}

// 删除商品对话框
void on_delete_product(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("删除商品", GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "确定", GTK_RESPONSE_ACCEPT,
                                                    "取消", GTK_RESPONSE_REJECT,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *id_entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("输入要删除的商品ID:"));
    gtk_container_add(GTK_CONTAINER(content), id_entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char *id_str = gtk_entry_get_text(GTK_ENTRY(id_entry));
        int id = atoi(id_str);

        // 输入验证
        if (id < 1 || id > count) {
            gtk_label_set_text(GTK_LABEL(status_label), "无效的商品ID！");
        } else {
            // 调用删除逻辑
            for (int i = id - 1; i < count - 1; i++) {
                products[i] = products[i + 1];
                products[i].id = i + 1;
            }
            count--;
            save_products();
            refresh_product_list(product_list);
            gtk_label_set_text(GTK_LABEL(status_label), "商品删除成功！");
        }
    }
    gtk_widget_destroy(dialog);
}

// 销售商品对话框
void on_sell_product(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("销售商品", GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "确定", GTK_RESPONSE_ACCEPT,
                                                    "取消", GTK_RESPONSE_REJECT,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *qty_entry = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("商品ID:"));
    gtk_container_add(GTK_CONTAINER(content), id_entry);
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("销售数量:"));
    gtk_container_add(GTK_CONTAINER(content), qty_entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        int id = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        int qty = atoi(gtk_entry_get_text(GTK_ENTRY(qty_entry)));

        if (id < 1 || id > count) {
            gtk_label_set_text(GTK_LABEL(status_label), "无效的商品ID！");
        } else if (qty <= 0) {
            gtk_label_set_text(GTK_LABEL(status_label), "数量必须大于0！");
        } else if (products[id-1].stock < qty) {
            gtk_label_set_text(GTK_LABEL(status_label), "库存不足！");
        } else {
            products[id-1].stock -= qty;
            products[id-1].sold += qty;
            save_products();
            refresh_product_list(product_list);
            gtk_label_set_text(GTK_LABEL(status_label), "销售成功！");
        }
    }
    gtk_widget_destroy(dialog);
}

// 补货商品对话框
void on_restock_product(GtkButton *button, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("补货商品", GTK_WINDOW(data),
                                                    GTK_DIALOG_MODAL,
                                                    "确定", GTK_RESPONSE_ACCEPT,
                                                    "取消", GTK_RESPONSE_REJECT,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *id_entry = gtk_entry_new();
    GtkWidget *qty_entry = gtk_entry_new();

    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("商品ID:"));
    gtk_container_add(GTK_CONTAINER(content), id_entry);
    gtk_container_add(GTK_CONTAINER(content), gtk_label_new("补货数量:"));
    gtk_container_add(GTK_CONTAINER(content), qty_entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        int id = atoi(gtk_entry_get_text(GTK_ENTRY(id_entry)));
        int qty = atoi(gtk_entry_get_text(GTK_ENTRY(qty_entry)));

        if (id < 1 || id > count) {
            gtk_label_set_text(GTK_LABEL(status_label), "无效的商品ID！");
        } else if (qty <= 0) {
            gtk_label_set_text(GTK_LABEL(status_label), "数量必须大于0！");
        } else {
            products[id-1].stock += qty;
            save_products();
            refresh_product_list(product_list);
            gtk_label_set_text(GTK_LABEL(status_label), "补货成功！");
        }
    }
    gtk_widget_destroy(dialog);
}

// 统计对话框
void on_statistics(GtkButton *button, gpointer data) {
    int total_sold = 0;
    double total_revenue = 0.0;

    for (int i = 0; i < count; i++) {
        total_sold += products[i].sold;
        total_revenue += products[i].sold * products[i].price;
    }

    char message[100];
    snprintf(message, sizeof(message),
             "总销售量: %d 件\n总收入: %.2f 元",
             total_sold, total_revenue);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// 主窗口初始化
void activate_app(GtkApplication *app, gpointer user_data) {
    // 创建主窗口
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "助农蔬菜销售系统");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // 创建布局容器
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // 工具栏按钮
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *add_btn = gtk_button_new_with_label("添加商品");
    g_signal_connect(add_btn, "clicked", G_CALLBACK(on_add_product), window);
    gtk_box_pack_start(GTK_BOX(toolbar), add_btn, FALSE, FALSE, 0);

    GtkWidget *del_btn = gtk_button_new_with_label("删除商品");
    g_signal_connect(del_btn, "clicked", G_CALLBACK(on_delete_product), window);

    GtkWidget *sell_btn = gtk_button_new_with_label("销售商品");
    g_signal_connect(sell_btn, "clicked", G_CALLBACK(on_sell_product), window);

    GtkWidget *restock_btn = gtk_button_new_with_label("补货商品");
    g_signal_connect(restock_btn, "clicked", G_CALLBACK(on_restock_product), window);

    GtkWidget *stat_btn = gtk_button_new_with_label("统计");
    g_signal_connect(stat_btn, "clicked", G_CALLBACK(on_statistics), window);

    gtk_box_pack_start(GTK_BOX(toolbar), del_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), sell_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), restock_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), stat_btn, FALSE, FALSE, 0);

    // 商品列表
    product_list = gtk_tree_view_new();
    GtkListStore *store = gtk_list_store_new(6, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(product_list), GTK_TREE_MODEL(store));

    // 添加列
    GtkCellRenderer *renderer;
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "ID", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "名称", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "价格", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "库存", renderer, "text", 3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "已售", renderer, "text", 4, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(product_list), -1, "添加时间", renderer, "text", 5, NULL);

    // 状态栏
    status_label = gtk_label_new("开始营业吧！");

    // 组装界面
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), product_list, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

    // 初始加载数据
    refresh_product_list(product_list);

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(product_list));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

    gtk_widget_show_all(window);
}



