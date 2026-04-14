#include "bool_array.hpp"
#include <cstring>  

BoolArray::BoolArray(size_t n, bool value) : num_bits(n)
{
    num_bytes = (n + 7) / 8;
    data = new unsigned char[num_bytes]();
    if (value)
    {
        for (size_t i = 0; i < n; ++i)
        {
            (*this)[i] = true;
        }
    }
}

BoolArray::BoolArray(const BoolArray &other) : num_bits(other.num_bits), num_bytes(other.num_bytes)
{
    data = new unsigned char[num_bytes];
    memcpy(data, other.data, num_bytes);
}

BoolArray &BoolArray::operator=(const BoolArray &other)
{
    if (this != &other)
    {
        delete[] data;
        num_bits = other.num_bits;
        num_bytes = other.num_bytes;
        data = new unsigned char[num_bytes];
        memcpy(data, other.data, num_bytes);
    }
    return *this;
}

bool BoolArray::operator[](size_t pos) const
{
    size_t byte = byte_index(pos);
    size_t bit = bit_offset(pos);
    return (data[byte] >> bit) & 1;
}

size_t BoolArray::size() const { return num_bits; }

BoolArray::Proxy::operator bool() const
{
    return (*byte >> bit) & 1;
}

BoolArray::Proxy &BoolArray::Proxy::operator=(bool value)
{
    if (value)
        *byte |= (1 << bit); // 1
    else
        *byte &= ~(1 << bit); // 0
    return *this;
}

BoolArray::Proxy BoolArray::operator[](size_t pos)
{
    return Proxy(&data[byte_index(pos)], bit_offset(pos));
}

void BoolArray::resize(size_t new_size, bool value)
{
    size_t new_bytes = (new_size + 7) / 8;
    unsigned char *new_data = new unsigned char[new_bytes]();

    size_t copy_bytes = num_bytes < new_bytes ? num_bytes : new_bytes;
    memcpy(new_data, data, copy_bytes);

    if (new_size > num_bits && value) // есди увеличиваем и нужно заполнить true
    {
        for (size_t i = num_bits; i < new_size; ++i)
        {
            size_t byte = i / 8;
            size_t bit = i % 8;
            new_data[byte] |= (1 << bit);
        }
    }

    delete[] data;
    data = new_data;
    num_bits = new_size;
    num_bytes = new_bytes;
}