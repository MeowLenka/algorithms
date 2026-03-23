#ifndef TEST_HELPER_HPP
#define TEST_HELPER_HPP

#include <cstdlib>

bool compare_buffers(const void* buf1, const void* buf2, size_t size);
char* generate_random_data(size_t size);


#endif