cmake_minimum_required(VERSION 4.0)
project(Lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab3 lab3.cpp)

# тестовые программы
add_executable(test_notpr_queue tests/test_notification_priority_queue.cpp)
target_link_libraries(test_notpr_queue PRIVATE notifprqueue)

# запуск всех тестов
add_custom_target(test
    COMMAND test_notpr_queue
    DEPENDS test_notpr_queue
    COMMENT "Running all tests..."
    VERBATIM
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lab3 PRIVATE -g -O0)
    target_link_options(lab3 PRIVATE -g -O0)
endif()

option(ENABLE_LEAKS "Enable AddressSanitizer" OFF)
if(ENABLE_LEAKS)
    target_compile_options(lab3 PRIVATE -fsanitize=address)
    target_link_options(lab3 PRIVATE -fsanitize=address)
endif()


add_library(mystring my_string.cpp)
add_library(myfile base_file.cpp)
add_library(boolarr bool_array.cpp)
add_library(matrix matrix.cpp)
add_library(notif notification.cpp)
add_library(notifqueue notification_queue.cpp)
add_library(notifprqueue notification_priority_queue.cpp)
add_library(workerdb worker_db.cpp)

target_include_directories(mystring PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(myfile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(boolarr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(matrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(notif PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(notifqueue PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(notifprqueue PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(workerdb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(lab3 PRIVATE mystring myfile boolarr matrix notif notifqueue notifprqueue workerdb)
