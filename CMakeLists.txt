cmake_minimum_required(VERSION 3.16)
project(CppDataStructures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(multilist_basic
    multilist-basic/main.cpp
    multilist-basic/Multilist.cpp
)
target_include_directories(multilist_basic PRIVATE multilist-basic)

add_executable(multilist_dimensional
    multilist-dimensional/main.cpp
    multilist-dimensional/dimensional.cpp
)
target_include_directories(multilist_dimensional PRIVATE multilist-dimensional)

add_executable(binary_search_tree
    binary-search-tree/main.cpp
    binary-search-tree/CustomBST.cpp
)
target_include_directories(binary_search_tree PRIVATE binary-search-tree)
