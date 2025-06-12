#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

User currentUser = {0};

// 初始化管理员账户
void add_admin_if_empty() {
    User admin = db_get_user_by_username("admin");
    if (admin.id == 0) {
        int admin_id = db_add_user("admin", "admin123", ROLE_ADMIN);
        if (admin_id > 0) {
            printf("已创建默认管理员账号: admin/admin123\n");
        }
    }
}

// 用户注册
int register_user() {
    char username[50], password[50];
    printf("请输入用户名: ");
    fflush(stdout);
    scanf("%49s", username);

    // 检查用户名是否存在
    User existing = db_get_user_by_username(username);
    if (existing.id != 0) {
        printf("用户名已存在!\n");
        return 0;
    }

    printf("请输入密码: ");
    fflush(stdout);
    scanf("%49s", password);

    int user_id = db_add_user(username, password, ROLE_USER);
    if (user_id > 0) {
        printf("注册成功! ID: %d\n", user_id);
        return 1;
    } else {
        printf("注册失败!\n");
        return 0;
    }
}

// 用户登录
int login() {
    char username[50], password[50];
    printf("用户名: ");
    fflush(stdout);
    scanf("%49s", username);

    printf("密码: ");
    fflush(stdout);
    scanf("%49s", password);

    User user = db_get_user_by_username(username);
    if (user.id == 0 || strcmp(user.password, password) != 0) {
        printf("用户名或密码错误!\n");
        return 0;
    }

    // 检查用户是否已登录
    if (db_is_user_logged(user.id)) {
        printf("该用户已在其他终端登录!\n");
        return 0;
    }

    currentUser = user;
    printf("登录成功! %s\n",
           currentUser.role == ROLE_ADMIN ? "[管理员]" : "");

    // 设置用户登录状态
    db_set_user_logged(user.id);

    return 1;
}

// 用户登出
void logout() {
    if (currentUser.id != 0) {
        printf("%s 已登出\n", currentUser.username);
        db_set_user_logged_out(currentUser.id);
        currentUser.id = 0;
    }
}

// 修改密码
void change_password() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char old_pwd[50], new_pwd[50], confirm_pwd[50];
    printf("请输入原密码: ");
    fflush(stdout);
    scanf("%49s", old_pwd);

    if (strcmp(old_pwd, currentUser.password) != 0) {
        printf("原密码错误!\n");
        return;
    }

    printf("请输入新密码: ");
    fflush(stdout);
    scanf("%49s", new_pwd);

    printf("请确认新密码: ");
    fflush(stdout);
    scanf("%49s", confirm_pwd);

    if (strcmp(new_pwd, confirm_pwd) != 0) {
        printf("两次输入的密码不一致!\n");
        return;
    }

    // 更新密码
    if (db_update_user(currentUser.id, new_pwd)) {
        strcpy(currentUser.password, new_pwd);
        printf("密码修改成功!\n");
    } else {
        printf("密码修改失败!\n");
    }
}

// 列出所有用户
void list_users() {
    if (currentUser.role != ROLE_ADMIN) {
        printf("权限不足!\n");
        return;
    }

    if (!db_list_users()) {
        printf("获取用户列表失败\n");
    }
}

// 删除用户
void delete_user() {
    if (currentUser.role != ROLE_ADMIN) {
        printf("权限不足!\n");
        return;
    }

    int id;
    printf("输入要删除的用户ID: ");
    fflush(stdout);
    scanf("%d", &id);

    if (id == currentUser.id) {
        printf("不能删除当前登录的用户!\n");
        return;
    }

    if (db_delete_user(id)) {
        printf("用户删除成功!\n");
    } else {
        printf("用户删除失败!\n");
    }
}