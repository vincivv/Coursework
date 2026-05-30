cmake_minimum_required(VERSION 3.10)

project(CacheManagerProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

add_executable(milestone5 
    binary_search_tree.cpp 
    milestone5.cpp
    cache_manager.cpp
    dll_node.cpp
    doubly_linked_list.cpp
    hash_node.cpp
    hash_table.cpp
    tree_node.cpp
)
target_include_directories(milestone5 PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

