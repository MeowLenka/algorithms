cmake_minimum_required(VERSION 4.0)

project(ExplicitLib)

add_library(explicit_lib SHARED cat_func.cpp) 


if(APPLE)
    set_target_properties(explicit_lib PROPERTIES
        OUTPUT_NAME "explicit_lib"
        SUFFIX ".dylib"
        INSTALL_RPATH "@executable_path"
    )
endif()