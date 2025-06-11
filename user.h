#ifndef USER_H
#define USER_H

#include "common.h"

// 用户结构体
typedef struct {
    int id;
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    UserRole role;
} User;

extern User users[];
extern int userCount;
extern User currentUser;

void register_user();
int login();
void change_password();
void list_users();
void delete_user();
void save_users();
void load_users();
void add_admin_if_empty();

#endif