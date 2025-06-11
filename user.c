#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "sha256.h"

// 全局用户数据
User users[MAX_USERS];
int userCount = 0;
User currentUser = {0};  // 当前登录用户

// 初始化管理员账户（如果没有用户）
void add_admin_if_empty() {
    if (userCount == 0) {
        User admin = {1, "admin", "", ROLE_ADMIN};
        sha256_hash("admin123", admin.password);
        users[userCount++] = admin;
        printf("已创建默认管理员账号: admin/admin123\n");
    }
}

// 用户注册
void register_user() {
    if (userCount >= MAX_USERS) {
        printf("用户数量已达上限!\n");
        return;
    }

    User newUser;
    printf("请输入用户名: ");
    scanf("%49s", newUser.username);

    // 检查用户名是否存在
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("用户名已存在!\n");
            return;
        }
    }

    char plainPwd[MAX_NAME_LEN];
    printf("请输入密码: ");
    scanf("%49s", plainPwd);

    sha256_hash(plainPwd, newUser.password);

    newUser.id = 1000 + userCount;
    newUser.role = ROLE_USER;  // 默认为普通用户

    users[userCount++] = newUser;
    printf("注册成功! ID: %d\n", newUser.id);

    save_users();
}

// 用户登录
int login() {
    char username[MAX_NAME_LEN], password[MAX_NAME_LEN];
    printf("用户名: ");
    scanf("%49s", username);
    printf("密码: ");
    scanf("%49s", password);

    char encrypted[65];
    sha256_hash(password, encrypted);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, encrypted) == 0) {
            currentUser = users[i];
            printf("登录成功! %s\n",
                   currentUser.role == ROLE_ADMIN ? "[管理员]" : "");
            return 1;
        }
    }
    printf("用户名或密码错误!\n");
    return 0;
}

// 修改密码
void change_password() {
    if (currentUser.id == 0) {
        printf("请先登录!\n");
        return;
    }

    char old_pwd[MAX_NAME_LEN], new_pwd[MAX_NAME_LEN], confirm_pwd[MAX_NAME_LEN];
    printf("请输入原密码: ");
    scanf("%49s", old_pwd);

    char old_encrypted[65];
    sha256_hash(old_pwd, old_encrypted);

    // 验证原密码
    if (strcmp(old_encrypted, currentUser.password) != 0) {
        printf("原密码错误!\n");
        return;
    }

    printf("请输入新密码: ");
    scanf("%49s", new_pwd);
    printf("请确认新密码: ");
    scanf("%49s", confirm_pwd);

    if (strcmp(new_pwd, confirm_pwd) != 0) {
        printf("两次输入的密码不一致!\n");
        return;
    }

    // 更新密码
    sha256_hash(new_pwd, currentUser.password);

    // 更新用户列表
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == currentUser.id) {
            strcpy(users[i].password, currentUser.password);
            break;
        }
    }

    save_users();
    printf("密码修改成功!\n");
}

// 列出所有用户（管理员）
void list_users() {
    if (currentUser.role != ROLE_ADMIN) {
        printf("权限不足!\n");
        return;
    }

    printf("\n%-6s %-20s %-10s\n", "ID", "用户名", "角色");
    printf("==================================\n");
    for (int i = 0; i < userCount; i++) {
        printf("%-6d %-20s %s\n",
               users[i].id,
               users[i].username,
               users[i].role == ROLE_ADMIN ? "管理员" : "普通用户");
    }
}

// 删除用户（管理员）
void delete_user() {
    if (currentUser.role != ROLE_ADMIN) {
        printf("权限不足!\n");
        return;
    }

    int id;
    printf("输入要删除的用户ID: ");
    scanf("%d", &id);

    if (id == currentUser.id) {
        printf("不能删除当前登录的用户!\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            // 从用户数组中移除
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

// 保存用户数据到文件
void save_users() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf("保存用户数据失败!\n");
        return;
    }

    fwrite(&userCount, sizeof(int), 1, fp);
    fwrite(users, sizeof(User), userCount, fp);
    fclose(fp);
}

// 从文件加载用户数据
void load_users() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return;

    fread(&userCount, sizeof(int), 1, fp);
    if (userCount > MAX_USERS) userCount = MAX_USERS;
    fread(users, sizeof(User), userCount, fp);
    fclose(fp);

    add_admin_if_empty();
}