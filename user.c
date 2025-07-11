#include <stdio.h>
#include <string.h>
#include "model.h"
#include "product.h"


#define USER_DATA_FILE "user_data.txt"  // 用户数据存储文件

User users[MAX_USERS];
int user_count = 10;
User current_user;


User get_current_user() {
    return current_user;
}

// 加载用户数据
void load_users() {
    FILE *fp = fopen(USER_DATA_FILE, "r");
    if (!fp) return;
    
    user_count = 0;
    while (fscanf(fp, "%d %s %s %d %d",
        &users[user_count].id,
        users[user_count].name,
        users[user_count].password,
        &users[user_count].is_admin,
        &users[user_count].is_login) == 5) {
        if (user_count++ >= MAX_USERS) break;
    }
    fclose(fp);
}

// 保存用户数据
void save_users() {
    FILE *fp = fopen(USER_DATA_FILE, "w");
    if (!fp) return;
    
    for (int i = 0; i < user_count; i++) {
        fprintf(fp, "%d %s %s %d %d\n",
            users[i].id,
            users[i].name,
            users[i].password,
            users[i].is_admin,
            users[i].is_login);
    }
    fclose(fp);
}

// 用户登录验证
int user_login() {
    load_users();
    char name[NAME_LEN], pwd[NAME_LEN];
    
    printf("请输入用户名：");
    fflush(stdout);
    scanf("%s", name);
    printf("请输入密码：");
    fflush(stdout);
    scanf("%s", pwd);
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, name) == 0) {
            if (strcmp(users[i].password, pwd) == 0) {
                if (users[i].is_login) {
                    printf("该用户已在其他终端登录！\n");
                    return 0;
                }
                current_user = users[i];
                current_user.is_login = 1;
                users[i].is_login = 1;
                save_users();
                load_products();
                return 1;
            }
            return 0;
        }
    }
    return 0;
}




// 添加用户（仅管理员）
void add_user() {
    if (!current_user.is_admin) {
        printf("无权限操作，需要管理员身份\n");
        return;
    }
    
    if (user_count >= MAX_USERS) {
        printf("用户数量已达上限\n");
        return;
    }
    
    User new_user;
    printf("输入新用户名：");
    fflush(stdout);  // 新增：刷新输出缓冲区
    scanf("%s", new_user.name);
    printf("输入密码：");
    fflush(stdout);
    scanf("%s", new_user.password);
    printf("是否为管理员（0-否，1-是）：");
    fflush(stdout);
    scanf("%d", &new_user.is_admin);
    new_user.is_login = 0;
    
    new_user.id = user_count + 1;
    users[user_count++] = new_user;
    save_users();
    printf("用户添加成功\n");
}

void show_users(){
    if (!current_user.is_admin) {
        printf("无权限操作，需要管理员身份\n");
    }
    for (int i = 0; i < user_count; i++) {
        printf("ID:%d 用户名：%s，密码：%s，是否为管理员：%d\n",users[i].id, users[i].name, users[i].password, users[i].is_admin);
    }
}

void get_user_data_file(char *buffer, size_t size) {
    snprintf(buffer, size, "data_user_%d.txt", get_current_user().id);
}