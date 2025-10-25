cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_HEADERS
    inlcude/CLI11.hpp
    include/tree.hpp
)

add_executable(avl_tree
    src/tree.cpp
)
target_include_directories(avl_tree PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Настройки компилятора
if(MSVC)
    target_compile_options(avl_tree PRIVATE /W4 /permissive-)
else()
    target_compile_options(avl_tree PRIVATE -Wall -Wextra -Wpedantic)
endif()
