cmake_minimum_required(VERSION 3.28)
project(System_vagetable_sale C)


# 添加MySQL头文件路径（请根据实际安装路径修改）
include_directories("C:/Program Files/MySQL/MySQL Server 8.0/include")

# 添加链接库路径（已存在的配置）
link_libraries(libmysql.dll)

# 如果库文件在非标准路径，还需要添加链接目录
link_directories("C:/Program Files/MySQL/MySQL Server 8.0/lib")

set(CMAKE_C_STANDARD 11)

add_executable(System_vagetable_sale main.c
        tool.c
        product.c
        product.h
        user.c
        common.h
        db.h
        db.c
)

target_link_libraries(
        System_vagetable_sale libmysql
)
