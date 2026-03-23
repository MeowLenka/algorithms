#include "test_helper.hpp"
#include <cstdlib>

bool compare_buffers(const void* buf1, const void* buf2, size_t size) {
    const char* b1 = static_cast<const char*>(buf1);
    const char* b2 = static_cast<const char*>(buf2);
    for (size_t i = 0; i < size; ++i) {
        if (b1[i] != b2[i]) return false;
    }
    return true;
}

char* generate_random_data(size_t size) {
    char* data = new char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<char>(rand() % 256);
    }
    return data;
}