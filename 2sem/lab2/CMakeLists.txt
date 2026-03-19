cmake_minimum_required(VERSION 4.0)
project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab2 src/lab2.cpp)

# Пытаемся найти внешнюю библиотеку
find_library(MyString
    NAMES my_string
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/string_lib/build
    NO_DEFAULT_PATH
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lab2 PRIVATE -g -O0)
    target_link_options(lab2 PRIVATE -g -O0)
endif()

option(ENABLE_LEAKS "Enable AddressSanitizer" OFF)
if(ENABLE_LEAKS)
    target_compile_options(program PRIVATE -fsanitize=address)
    target_link_options(program PRIVATE -fsanitize=address)
endif()

# Проверяем, нашлась ли библиотека
if(NOT MyString)
    message(FATAL_ERROR "Внешняя библиотека не найдена! Путь: ${CMAKE_CURRENT_SOURCE_DIR}/string_lib/build")
endif()

message(STATUS "Найдена внешняя библиотека: ${MyString}")

add_library(myfile src/base_file.cpp)

target_include_directories(myfile PRIVATE $
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Указываем где искать заголовочные файлы внешней библиотеки
target_include_directories(lab2 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/string_lib/src
)

# Линкуем внешнюю библиотеку
target_link_libraries(lab2 PRIVATE myfile ${MyString})

