set(LIB_PATH ${CMAKE_SOURCE_DIR}/src)

# макрос для создания простого теста
function(add_simple_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} lab3_lib)
    target_include_directories(${TEST_NAME} PRIVATE ${LIB_PATH})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_simple_test(test_my_string test_my_string.cpp)
add_simple_test(test_matrix test_matrix.cpp)
add_simple_test(test_bool_array test_bool_array.cpp)
add_simple_test(test_notification test_notification.cpp)
add_simple_test(test_worker_db test_worker_db.cpp)
