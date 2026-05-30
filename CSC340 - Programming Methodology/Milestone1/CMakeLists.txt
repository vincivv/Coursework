cmake_minimum_required(VERSION 3.0.0)

project(DoublyLinkedListProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

add_executable(milestone1 
    milestone1.cpp 
    doubly_linked_list.cpp 
    generate_output.cpp 
    dll_node.cpp
)
target_include_directories(milestone1 PRIVATE ${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

