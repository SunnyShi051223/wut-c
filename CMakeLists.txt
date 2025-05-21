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
