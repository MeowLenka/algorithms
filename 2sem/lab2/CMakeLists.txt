cmake_minimum_required(VERSION 4.0)
project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab2 src/lab2.cpp)

# Вспомогательная библиотека для тестов
add_library(test_helper STATIC tests/test_helper.cpp)
target_include_directories(test_helper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Отдельные тестовые программы
add_executable(test_base_file tests/test_base_file.cpp)
target_link_libraries(test_base_file PRIVATE myfile test_helper)

add_executable(test_base32_file tests/test_base32_file.cpp)
target_link_libraries(test_base32_file PRIVATE myfile test_helper)

add_executable(test_rle_file tests/test_rle_file.cpp)
target_link_libraries(test_rle_file PRIVATE myfile test_helper)

# Цель для запуска всех тестов
add_custom_target(test
    COMMAND test_base_file
    COMMAND test_base32_file
    COMMAND test_rle_file
    DEPENDS test_base_file test_base32_file test_rle_file
    COMMENT "Running all tests..."
    VERBATIM
)

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
    target_compile_options(lab2 PRIVATE -fsanitize=address)
    target_link_options(lab2 PRIVATE -fsanitize=address)
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

