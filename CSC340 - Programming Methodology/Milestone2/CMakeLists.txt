cmake_minimum_required(VERSION 3.10)

project(HashTableProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

add_executable(milestone2 
    milestone2.cpp 
    hash_table.cpp
)
target_include_directories(milestone2 PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

