#include <iostream>
#include <cstdio>
#include <cstring>
#include "base_file.hpp"

BaseFile::BaseFile() : _file(nullptr) {}

BaseFile::BaseFile(const char *filename, const char *mode) : BaseFile(fopen(filename, mode)) {}

BaseFile::BaseFile(FILE *file) : _file(file) {};

BaseFile::~BaseFile()
{
    fclose(_file);
}

bool BaseFile::is_open()
{
    return !_file;
}

bool BaseFile::can_read()
{
    if (!_file || feof(_file))
        return false;
    ifstream _file;
    return _file.is_open() && _file.good();
}

bool BaseFile::can_write()
{
    if (!_file || feof(_file))
        return false;

    _file << " "; // пробная запись
    if (_file.fail())
        return false;
    _file.seekp(-1, fstream::end);
    return true;
}

size_t BaseFile::write_raw(const void *buf, size_t n_bytes)
{
    if (!is_open() || !buf || !can_write())
        return 0;

    return fwrite(buf, 1, n_bytes, _file);
}

size_t BaseFile::read_raw(void *buf, size_t max_bytes)
{
    if (!is_open() || !buf || !can_read())
        return 0;

    return fread(buf, 1, max_bytes, _file);
}

long BaseFile::tell()
{
    if (!is_open())
        return -1L;
    return ftell(_file);
}

bool BaseFile::seek(long offset)
{
    if (!is_open())
        return false;
    return fseek(_file, offset, SEEK_SET) == 0;
}

size_t BaseFile::write(const void *buf, size_t n_bytes) { write_raw(buf, n_bytes) };

size_t BaseFile::read(void *buf, size_t max_bytes) { read_raw(buf, max_bytes) };

//////////

Base32File::Base32File(const char *table) : BaseFile(fopen(filename, mode), table) {}

Base32File::Base32File(const char *filename, const char *mode, const char *table) : BaseFile(fopen(filename, mode), table) {}

Base32File::Base32File(FILE *file, const char *table) : _file(file), _table{table}
{
    if (strlen(table) != 32)
        throw "Encoding table size error";

    create_decoding_table();
}

Base32File::~Base32File()
{
    fclose(_file);
}

size_t Base32File::write(const void *buf, size_t n_bytes)
{
    size_t dst_size = encoded32_size(n_bytes);
    char *dst = new char[dst_size];
    size_t written = 0;

    if (encode32(buf, dst_size, dst) == 0)
    {
        written = write_raw(dst, dst_size);
    }
    delete[] dst;
    return written;
}

size_t Base32File::read(void *buf, size_t max_bytes)
{
    size_t readen_size = read_raw(buf, max_bytes);
    size_t dst_size = decoded32_size(readen_size);
    char *dst = new char[dst_size];

    decode32(buf, readen_size, dst);
    memcpy(buf, dst, dst_size);
    delete[] dst;
    return dst_size;
}

size_t Base32File::encoded32_size(size_t raw_size)
{
    return ceil((raw_size * 8 + 4) / 5);
}

size_t Base32File::decoded32_size(size_t encode_size)
{
    return encode_size * 5 / 8;
}

int Base32File::encode32(const char *raw_data, size_t raw_size, char *dst)
{
    if (!raw_data || !dst || raw_size < 0)
    {
        return 1;
    }
    char buf = 0;
    int count = 0;
    for (int i = 0; i < raw_size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            buf += ((raw_data[i] >> j) & 1) << (4 - (count++ % 5));
            if (count % 5 == 0)
            {
                dst[(count / 5) - 1] = _table[buf];
                buf = 0;
            }
        }
    }
    if (count % 5)
    {
        dst[(count / 5)] = _table[buf];
    }

    return 0;
}

int Base32File::decode32(const char *encoded_data, size_t encoded_size, char *dst)
{

    if (!encoded_data || !dst || encoded_size < 0)
    {
        return 1;
    }
    char buf = 0;
    int count = 0;
    for (int i = 0; i < encoded_size; i++)
    {
        char letter = encoded_data[i];
        if (_decoding_table[letter] == -1)
            return 2;
        for (int j = 4; j >= 0; j--)
        {
            buf += ((_decoding_table[letter] >> j) & 1) << (7 - (count++ % 8));
            if (count % 8 == 0)
            {
                dst[(count / 8) - 1] = buf;
                buf = 0;
            }
        }
    }
    return 0;
}

int Base32File::create_decoding_table()
{
    for (int i = 0; i < 32; i++)
        _decoding_table[_table[i]] = i;
    return 0;
}

/////////////////////// RLE

RleFile::RleFile() : _file(nullptr) {}

RleFile::RleFile(const char *filename, const char *mode) : RleFile(fopen(filename, mode)) {}

RleFile::RleFile(FILE *file) : _file(file) {}

RleFile::~RleFile()
{
    fclose(_file);
}

size_t RleFile::write(const void *buf, size_t n_bytes)
{
    char *encoded_str = new char[n_bytes * 2];
    size_t n = (buf, n_bytes, encoded_str);
    write_raw(encoded_str, n);
    delete[] encoded_str;
    return n;
}

size_t RleFile::encode(const void *src, size_t n_bytes, char *dst)
{
    const char *buf = (char *)src;
    char *encoded_str = (char *)dst;
    size_t n_result = 0, count;

    for (int i = 0; i < n_bytes; i++)
    {
        count = 1;
        while (i + 1 < n_bytes && buf[i] == buf[i + 1])
        {
            count++;
            i++;
        }
        if (count > 1)
        {
            int written = snprintf(encoded_str + n_result, (n_bytes * 3), "%d", count);
            n_result += written;
        }
        encoded_str[n_result++] = buf[i];
    }
    return n_result;
}

size_t RleFile::read(const void *dst, size_t max_bytes)
{
    if (!dst)
        return NULL;
    char *buf = (char *)dst; // output
    char let;

    int output_index = 0;
    size_t i = 0, n_result = 0;

    while (!feof(_file) && n_result < max_bytes - 1)
    {
        let = static_cast<char>(getc(_file));

        int count = 0;
        if (let >= '0' && let <= '9')
        {
            while (!feof(_file) && let >= '0' && let <= '9')
            {
                count = (count) * 10 + (let - '0');
                let = static_cast<char>(getc(_file));
            }
        }
        if (count < max_bytes - n_result)
        {
            if (count == 0)
            {
                buf[n_result++] = let;
            }
            else
            {
                for (int j = 0; j < count; j++)
                {
                    buf[n_result++] = let;
                }
            }
        }
        else
        {
            buf[n_result] = '\0';
            return n_result;
        }
    }
    buf[n_result] = '\0';
    return n_result;
}