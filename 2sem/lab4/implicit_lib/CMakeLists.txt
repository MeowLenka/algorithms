cmake_minimum_required(VERSION 4.0)

project(ImplicitLib)

add_library(implicit_lib SHARED circle_func.cpp)
target_include_directories(implicit_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
    set_target_properties(implicit_lib PROPERTIES
        SUFFIX ".dylib"
        INSTALL_RPATH "@executable_path"
    )
endif()