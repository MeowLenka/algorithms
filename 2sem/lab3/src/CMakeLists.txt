set(SOURCES
    lab3.cpp
    my_string.cpp
    matrix.cpp
    base_file.cpp
    bool_array.cpp
    worker_db.cpp
    notification.cpp
    notification_queue.cpp
    notification_priority_queue.cpp
)

set(HEADERS
    my_string.hpp
    matrix.hpp
    base_file.hpp
    bool_array.hpp
    worker_db.hpp
    notification.hpp
    notification_queue.hpp
    notification_priority_queue.hpp
)

# статическая библиотеки для тестирования
add_library(lab3_lib STATIC ${SOURCES} ${HEADERS})

# Установка свойств для библиотеки
target_include_directories(lab3_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Если нужно собрать исполняемый файл (опционально)
add_executable(lab3 lab3.cpp)
target_link_libraries(lab3 lab3_lib)