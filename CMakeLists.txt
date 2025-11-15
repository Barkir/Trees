cmake_minimum_required(VERSION 3.16)
project(LRUCache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_HEADERS
    inlcude/CLI11.hpp
    include/tree.hpp
)

add_executable(avl_tree_regular
    src/tree.cpp
)

add_executable(avl_tree_timebench
    src/benchmarks.cpp
)

add_executable(set_timebench
    src/set_benchmark.cpp
)

target_include_directories(avl_tree_regular PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(avl_tree_timebench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(set_timebench PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)




# Настройки компилятора
if(MSVC)
    target_compile_options(avl_tree_regular PRIVATE /W4 /permissive-)
    target_compile_options(avl_tree_timebench PRIVATE /W4 /permissive-)
    target_compile_options(set_timebench PRIVATE /W4 /permissive-)

else()
    target_compile_options(avl_tree_regular PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(avl_tree_timebench PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(set_timebench PRIVATE -Wall -Wextra -Wpedantic)

endif()
