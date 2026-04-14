#ifndef BOOL_ARRAY_HPP
#define BOOL_ARRAY_HPP

#include <cstddef>  // size_t

class BoolArray
{
private:
    unsigned char *data;
    size_t num_bits;
    size_t num_bytes;

    size_t byte_index(size_t pos) const { return pos / 8; }
    size_t bit_offset(size_t pos) const { return pos % 8; }

public:
    BoolArray(size_t n, bool value = false);
    BoolArray(const BoolArray &other);
    ~BoolArray() { delete[] data; }


    BoolArray &operator=(const BoolArray &other);
    bool operator[](size_t pos) const;
    size_t size() const;
    
    class Proxy // хранит указатель на байт и номер бита
    {
    private:
        unsigned char *byte;
        size_t bit;

    public:
        Proxy(unsigned char *b, size_t bt) : byte(b), bit(bt) {}
        operator bool() const;
        Proxy &operator=(bool value);
    };

    Proxy operator[](size_t pos);

    void resize(size_t new_size, bool value = false);
};

#endif