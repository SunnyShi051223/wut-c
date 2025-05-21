cmake_minimum_required(VERSION 3.28)
project(System_vagetable_sale C)

set(CMAKE_C_STANDARD 11)

add_executable(System_vagetable_sale main.c
        tool.c
        product.c
        product.h
        model.h
        window.c
)

FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
TARGET_LINK_LIBRARIES(System_vagetable_sale ${GTK3_LIBRARIES})
