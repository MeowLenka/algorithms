cmake_minimum_required(VERSION 3.20) 
project(PriceDB)

set(CMAKE_C_STANDARD 11)

set(SOURCES
    src/main.c
    src/price.c
)

add_executable(price_db ${SOURCES})

target_include_directories(price_db PRIVATE src)

find_package(ZLIB REQUIRED)

target_link_libraries(price_db
    ncurses
    ${ZLIB_LIBRARIES}
    m
)
