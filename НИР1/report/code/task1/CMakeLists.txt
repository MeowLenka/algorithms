cmake_minimum_required(VERSION 3.20)
project(PalindromeProject VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_STATIC_LIB "Build static library" ON)
option(BUILD_SHARED_LIB "Build shared library" ON)
option(BUILD_TESTS "Build test executables" ON)
option(BUILD_DLOPEN_TEST "Build dlopen test" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

enable_testing()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
