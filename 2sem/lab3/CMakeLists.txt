cmake_minimum_required(VERSION 4.0)
project(Lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# санитайзеры для отладки
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    option(USE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
    if(USE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
    endif()
endif()


# директория с исходниками
add_subdirectory(src)

# тесто
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()