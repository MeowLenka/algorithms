cmake_minimum_required(VERSION 4.1 FATAL_ERROR)

project(proj)

add_executable(${PROJECT_NAME}
    src/lab2.cpp
)



add_library(proj_lib STATIC
    src/my_string.cpp
)

target_link_libraries(${PROJECT_NAME} proj_lib)

target_include_directories(proj_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
