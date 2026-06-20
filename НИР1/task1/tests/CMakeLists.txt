# тест со статической линковкой
add_executable(test_palindrome test_palindrome.c)

# определение макроса STATIC_BUILD для статической версии
target_compile_definitions(test_palindrome PRIVATE STATIC_BUILD)

target_link_libraries(test_palindrome palindrome_static)
target_include_directories(test_palindrome PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME test_palindrome_static COMMAND test_palindrome 1000 10 iter)

# тест с динамической линковкой
add_executable(test_palindrome_dynamic test_palindrome.c)

# НЕ определяем STATIC_BUILD для динамической версии
target_link_libraries(test_palindrome_dynamic palindrome_shared)
target_include_directories(test_palindrome_dynamic PRIVATE ${CMAKE_SOURCE_DIR}/src)


if(APPLE)
    set_target_properties(test_palindrome_dynamic PROPERTIES
        BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
    )
else()
    set_target_properties(test_palindrome_dynamic PROPERTIES
        BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif()

