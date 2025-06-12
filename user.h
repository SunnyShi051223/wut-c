#ifndef USER_H
#define USER_H

#include "common.h"
#include "db.h"

// 当前用户
extern User currentUser;

int register_user();
int login();
void logout();
void change_password();
void list_users();
void delete_user();
void add_admin_if_empty();

#endif