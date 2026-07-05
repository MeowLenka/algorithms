set(LIB_SOURCES palindrome.c)
set(LIB_HEADERS palindrome.h)

# статическая библиотека
if(BUILD_STATIC_LIB)
    add_library(palindrome_static STATIC ${LIB_SOURCES})
    target_include_directories(palindrome_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(palindrome_static PROPERTIES
        OUTPUT_NAME palindrome
        CLEAN_DIRECT_OUTPUT 1
    )
    install(TARGETS palindrome_static
        ARCHIVE DESTINATION lib
    )
endif()

# динамическая библиотека
if(BUILD_SHARED_LIB)
    add_library(palindrome_shared SHARED ${LIB_SOURCES})
    target_include_directories(palindrome_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(palindrome_shared PROPERTIES
        OUTPUT_NAME palindrome
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)
    
    target_link_options(palindrome_shared PRIVATE 
        -Wl,-soname,libpalindrome.so.1
    
    install(TARGETS palindrome_shared
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
endif()

install(FILES ${LIB_HEADERS} DESTINATION include)
