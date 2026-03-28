#include "../src/base_file.hpp"
#include "test_helper.hpp"
#include <iostream>
#include <cassert>


int main() {
    std::cout << "Running RleFile test\n";

    const size_t DATA_SIZE = 50000;
    const size_t BUFFER_SIZE = 256;    
    char* original_data = generate_random_data(DATA_SIZE);

    RleFile writer("test_rle.txt", "w");
    size_t total_written = 0;
    while (total_written < DATA_SIZE) {
        size_t to_write = (DATA_SIZE - total_written < BUFFER_SIZE) ? 
                          DATA_SIZE - total_written : BUFFER_SIZE;
        size_t written = writer.write(original_data + total_written, to_write);
        assert(written > 0);
        total_written += to_write;
    }

    RleFile reader("test_rle.txt", "r");
    char* read_data = new char[DATA_SIZE];
    size_t total_read = 0;
    while (total_read < DATA_SIZE) {
        size_t to_read = (DATA_SIZE - total_read < BUFFER_SIZE) ? 
                         DATA_SIZE - total_read : BUFFER_SIZE;
        size_t read = reader.read(read_data + total_read, to_read);
        total_read += read;
    }
    assert(total_read == DATA_SIZE);
    
    assert(compare_buffers(original_data, read_data, DATA_SIZE));
    
    delete[] original_data;
    delete[] read_data;
    std::cout << "RleFile test PASSED\n";
    return 0;
}
