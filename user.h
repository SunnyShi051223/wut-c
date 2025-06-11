#ifndef SYSTEM_VAGETABLE_SALE_USER_H
#define SYSTEM_VAGETABLE_SALE_USER_H

#include "common.h"

// 用户结构体
typedef struct {
    int id;
    char username[MAX_NAME_LEN];
    char password[65];
    UserRole role;
} User;

void load_users();
void save_users();
int user_login();
void add_user();
void show_users();

extern User current_user;
extern int user_count;
extern User users[MAX_USERS];

User get_current_user(void);
void get_user_data_file(char *buffer, size_t size);


#endif