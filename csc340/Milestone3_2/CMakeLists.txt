cmake_minimum_required(VERSION 3.10)

project(CacheManagerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(milestone3 
    milestone3.cpp 
    cache_manager.cpp 
    doubly_linked_list.cpp 
    hash_table.cpp
    dll_node.cpp
    hash_node.cpp
)

target_include_directories(milestone3 PRIVATE ${CMAKE_SOURCE_DIR})
