#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

User users[MAX_USERS];
int userCount = 0;
User currentUser = {0};

// 初始化管理员账户
void add_admin_if_empty() {
    if (userCount == 0) {
        User admin = {1, "admin", "admin123", ROLE_ADMIN};
        users[userCount++] = admin;
        printf("已创建默认管理员账号: admin/admin123\n");
        save_users(); // 立即保存新用户
    }
}

// 检查用户是否已登录
int is_user_logged(int user_id) {
    FILE *fp = fopen(LOGGED_USERS_FILE, "r");
    if (!fp) return 0; // 文件不存在则认为未登录

    int logged_id;
    while (fscanf(fp, "%d", &logged_id) != EOF) {
        if (logged_id == user_id) {
            fclose(fp);
            return 1; // 用户已登录
        }
    }
    fclose(fp);
    return 0; // 用户未登录
}

// 保存已登录用户
void save_logged_users() {
    FILE *fp = fopen(LOGGED_USERS_FILE, "w");
    if (!fp) return;

    // 当前登录用户
    if (currentUser.id != 0) {
        fprintf(fp, "%d\n", currentUser.id);
    }
    fclose(fp);
}

// 用户注册
void register_user() {
    if (userCount >= MAX_USERS) {
        printf("用户数量已达上限!\n");
        return;
    }

    User newUser;
    printf("请输入用户名: ");
    fflush(stdout);
    scanf("%49s", newUser.username);

    // 检查用户名是否存在
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("用户名已存在!\n");
            return;
        }
    }

    printf("请输入密码: ");
    fflush(stdout);
    scanf("%49s", newUser.password);

    newUser.id = 1000 + userCount;
    newUser.role = ROLE_USER;
    users[userCount++] = newUser;
    printf("注册成功! ID: %d\n", newUser.id);

    save_users();
}

// 用户登录
int login() {
    char username[MAX_NAME_LEN], password[MAX_NAME_LEN];
    printf("用户名: ");
    fflush(stdout);
    scanf("%49s", username);

    printf("密码: ");
    fflush(stdout);
    scanf("%49s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {

            // 检查用户是否已在其他终端登录
            if (is_user_logged(users[i].id)) {
                printf("该用户已在其他终端登录，请勿重复登录!\n");
                return 0;
            }

            currentUser = users[i];
            printf("登录成功! %s\n",
                   currentUser.role == ROLE_ADMIN ? "[管理员]" : "");

            // 保存登录状态
            save_logged_users();
            return 1;
        }
    }
    printf("用户名或密码错误!\n");
    return 0;
}

// 用户登出
void logout() {
    if (currentUser.id != 0) {
        printf("%s 已登出\n", currentUser.username);

        // 清除登录状态
        FILE *fp = fopen(LOGGED_USERS_FILE, "w");
        if (fp) fclose(fp);

        currentUser.id = 0;
    }
}

// 修改密码
void change_password() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char old_pwd[MAX_NAME_LEN], new_pwd[MAX_NAME_LEN], confirm_pwd[MAX_NAME_LEN];
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

    strcpy(currentUser.password, new_pwd);

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == currentUser.id) {
            strcpy(users[i].password, new_pwd);
            break;
        }
    }

    save_users();
    printf("密码修改成功!\n");
}

// 列出所有用户
void list_users() {
    if (currentUser.role != ROLE_ADMIN) {
        printf("权限不足!\n");
        return;
    }

    printf("\n%-6s %-20s %-10s %-8s\n", "ID", "用户名", "角色", "状态");
    printf("===========================================\n");
    for (int i = 0; i < userCount; i++) {
        printf("%-6d %-20s %-10s %s\n",
               users[i].id,
               users[i].username,
               users[i].role == ROLE_ADMIN ? "管理员" : "普通用户",
               is_user_logged(users[i].id) ? "在线" : "离线");
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

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;

            save_users();
            printf("用户删除成功!\n");
            return;
        }
    }

    printf("未找到该用户!\n");
}

// 保存用户数据到文本文件
void save_users() {
    FILE *fp = fopen(USER_FILE, "w");
    if (!fp) {
        printf("保存用户数据失败!\n");
        return;
    }

    fprintf(fp, "%d\n", userCount);
    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%d %s %s %d\n",
                users[i].id,
                users[i].username,
                users[i].password,
                users[i].role);
    }
    fclose(fp);
}

// 从文本文件加载用户数据
void load_users() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        // 文件不存在时创建默认管理员
        add_admin_if_empty();
        return;
    }

    fscanf(fp, "%d", &userCount);
    if (userCount > MAX_USERS) userCount = MAX_USERS;

    for (int i = 0; i < userCount; i++) {
        fscanf(fp, "%d %s %s %d",
               &users[i].id,
               users[i].username,
               users[i].password,
               (int*)&users[i].role);
    }
    fclose(fp);
}