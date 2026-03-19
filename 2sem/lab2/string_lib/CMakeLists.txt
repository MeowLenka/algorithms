cmake_minimum_required(VERSION 4.0)

project(MyString)

add_library(my_string STATIC src/my_string.cpp)
target_include_directories(my_string PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
