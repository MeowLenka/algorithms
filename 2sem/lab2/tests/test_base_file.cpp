#include "../src/base_file.hpp"
#include "test_helper.hpp"
#include <iostream>
#include <cassert>


int main()
{
    std::cout << "\n=== Testing BaseFile ===\n";

    const size_t DATA_SIZE = 50000; // > 40KB
    const size_t BUFFER_SIZE = 256;

    // Генерируем случайные данные
    char *original_data = generate_random_data(DATA_SIZE);

    // Записываем
    BaseFile writer("test_base.txt", "w");
    size_t total_written = 0;
    while (total_written < DATA_SIZE)
    {
        size_t to_write = (DATA_SIZE - total_written < BUFFER_SIZE) ? DATA_SIZE - total_written : BUFFER_SIZE;
        size_t written = writer.write(original_data + total_written, to_write);
        assert(written == to_write);
        total_written += written;
    }
    assert(total_written == DATA_SIZE);

    // Читаем
    BaseFile reader("test_base.txt", "r");
    char *read_data = new char[DATA_SIZE];
    size_t total_read = 0;
    while (total_read < DATA_SIZE)
    {
        size_t to_read = (DATA_SIZE - total_read < BUFFER_SIZE) ? DATA_SIZE - total_read : BUFFER_SIZE;
        size_t read = reader.read(read_data + total_read, to_read);
        assert(read == to_read);
        total_read += read;
    }
    assert(total_read == DATA_SIZE);

    // // Сравниваем
    assert(compare_buffers(original_data, read_data, DATA_SIZE));

    // delete[] original_data;
    delete[] read_data;
    std::cout << "BaseFile test PASSED\n";

    return 0;   
}