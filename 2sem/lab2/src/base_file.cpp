#include <iostream>
#include <cstdio>
#include <cstring>
#include "base_file.hpp"

BaseFile::BaseFile() : _file(nullptr) {}

BaseFile::BaseFile(const char *filename, const char *mode) : BaseFile(fopen(filename, mode)) {}

BaseFile::BaseFile(FILE *file) : _file(file) {}

BaseFile::~BaseFile()
{
    std::cout << "basefile destr\n";
    fclose(_file);
}

bool BaseFile::is_open()
{
    return _file != nullptr;
}

bool BaseFile::can_read()
{
    if (!_file || feof(_file))
        return false;

    int c = fgetc(_file);
    if (c == EOF)
        return false;

    ungetc(c, _file);
    return !ferror(_file);
}

bool BaseFile::can_write()
{
    if (!_file)
        return false;

    long pos = ftell(_file);
    int result = fputc(' ', _file);
    if (result == EOF)
        return false;

    fseek(_file, pos, SEEK_SET);

    return true;
}

size_t BaseFile::write_raw(const void *buf, size_t n_bytes)
{
    return fwrite(buf, 1, n_bytes, _file);
}

size_t BaseFile::read_raw(void *buf, size_t max_bytes)
{
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

size_t BaseFile::write(const void *buf, size_t n_bytes) { return write_raw(buf, n_bytes); }

size_t BaseFile::read(void *buf, size_t max_bytes) { return read_raw(buf, max_bytes); }

//////////

Base32File::Base32File(const char *table) : Base32File{(FILE *)nullptr, table} {}

Base32File::Base32File(const char *filename, const char *mode, const char *table) : Base32File(fopen(filename, mode), table) {}

Base32File::Base32File(FILE *file, const char *table) : BaseFile(file), _table{table}
{
    if (strlen(table) != 32)
        throw "Encoding table size error";

    create_decoding_table();
}

Base32File::~Base32File() { std::cout << "base32file dest\n"; }

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

int Base32File::encode32(const void *raw_data, size_t raw_size, char *dst)
{
    if (!raw_data || !dst || raw_size < 0)
    {
        return 1;
    }
    const unsigned char *raw_ptr = (unsigned char *)raw_data;

    char buf = 0;
    int count = 0;
    for (int i = 0; i < raw_size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            buf += ((raw_ptr[i] >> j) & 1) << (4 - (count++ % 5));
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

int Base32File::decode32(const void *encoded_data, size_t encoded_size, char *dst)
{

    if (!encoded_data || !dst || encoded_size < 0)
    {
        return 1;
    }
    const char *encoded_ptr = (const char *)encoded_data;
    char buf = 0;
    int count = 0;
    for (int i = 0; i < encoded_size; i++)
    {
        char letter = encoded_ptr[i];
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

RleFile::RleFile() {};

RleFile::RleFile(const char *filename, const char *mode) : RleFile(fopen(filename, mode)) {}

RleFile::RleFile(FILE *file) : BaseFile(file) {}

RleFile::~RleFile() { std::cout << "rlefile dest\n"; }

size_t RleFile::write(const void *buf, size_t n_bytes)
{
    char *encoded_str = new char[n_bytes * 2];
    size_t n = encode(buf, n_bytes, encoded_str);
    write_raw(encoded_str, n);
    delete[] encoded_str;
    return n;
}

size_t RleFile::encode(const void *src, size_t n_bytes, char *dst)
{
    const char *buf = (char *)src;
    char *encoded_str = (char *)dst;
    size_t i = 0, n_result = 0;
    int count;

    while (i < n_bytes)
    {
        count = 1;
        const char cur = buf[i++];
        while (cur == buf[i])
        {
            count++;
            i++;
        }
        encoded_str[n_result++] = count;
        encoded_str[n_result++] = cur;
    }

    // for (int i = 0; i < n_bytes; i++)
    // {
    //     count = 1;
    //     while (i + 1 < n_bytes && buf[i] == buf[i + 1])
    //     {
    //         count++;
    //         i++;
    //     }
    //     if (count > 1)
    //     {
    //         int written = snprintf(encoded_str + n_result, (n_bytes * 3), "%d", count);
    //         n_result += written;
    //     }
    //     encoded_str[n_result++] = buf[i];
    // }
    // encoded_str[n_result - 1] = '\0';

    return n_result;
}

size_t RleFile::read(void *dst, size_t max_bytes)
{
    if (!dst)
        return 0;

    char *buf = (char *)dst;
    char count, c;
    size_t n_result = 0;
    while (!feof(_file) && n_result < max_bytes - 1)
    {
        count = getc(_file);
        if (n_result + count > max_bytes)
        {
            ungetc(count, _file);
            return n_result;
        }
        char c = getc(_file);
        for (int j = 0; j < count; j++)
            buf[n_result++] = c;
    }
    return n_result;
}

//////// 2 ////////

Base32File2::Base32File2(IFile *file, const char *table) : _file(file)
{
    if (strlen(table) != 32)
        throw "Encoding table size error";

    create_decoding_table();
}

Base32File2::~Base32File2() { delete _file; }

bool Base32File2::can_read() { return _file->can_read(); }

bool Base32File2::can_write() { return _file->can_write(); }

size_t Base32File2::write(const void *buf, size_t n_bytes)
{
    size_t dst_size = encoded32_size(n_bytes);
    char *dst = new char[dst_size];
    size_t written = 0;

    if (encode32(buf, dst_size, dst) == 0)
    {
        written = _file->write(dst, dst_size);
    }
    delete[] dst;
    return written;
}

size_t Base32File2::read(void *buf, size_t max_bytes)
{
    size_t readen_size = _file->read(buf, max_bytes);
    size_t dst_size = decoded32_size(readen_size);
    char *dst = new char[dst_size];

    decode32(buf, readen_size, dst);
    memcpy(buf, dst, dst_size);
    delete[] dst;
    return dst_size;
}

size_t Base32File2::encoded32_size(size_t raw_size)
{
    return ceil((raw_size * 8 + 4) / 5);
}

size_t Base32File2::decoded32_size(size_t encode_size)
{
    return encode_size * 5 / 8;
}

int Base32File2::encode32(const void *raw_data, size_t raw_size, char *dst)
{
    if (!raw_data || !dst || raw_size < 0)
    {
        return 1;
    }
    const unsigned char *raw_ptr = (unsigned char *)raw_data;

    char buf = 0;
    int count = 0;
    for (int i = 0; i < raw_size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            buf += ((raw_ptr[i] >> j) & 1) << (4 - (count++ % 5));
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

int Base32File2::decode32(const void *encoded_data, size_t encoded_size, char *dst)
{

    if (!encoded_data || !dst || encoded_size < 0)
    {
        return 1;
    }
    const char *encoded_ptr = (const char *)encoded_data;
    char buf = 0;
    int count = 0;
    for (int i = 0; i < encoded_size; i++)
    {
        char letter = encoded_ptr[i];
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

int Base32File2::create_decoding_table()
{
    for (int i = 0; i < 32; i++)
        _decoding_table[_table[i]] = i;
    return 0;
}

/////////////////2/////////////////

RleFile2::RleFile2(IFile *file) : _file(file) {};

RleFile2::~RleFile2() { delete _file; }

bool RleFile2::can_read() { return _file->can_read(); }
bool RleFile2::can_write() { return _file->can_write(); }

size_t RleFile2::write(const void *buf, size_t n_bytes)
{
    char *encoded_str = new char[n_bytes * 2];
    size_t n = encode(buf, n_bytes, encoded_str);
    _file->write(encoded_str, n);
    delete[] encoded_str;
    return n;
}

size_t RleFile2::encode(const void *src, size_t n_bytes, char *dst)
{
    const char *buf = (char *)src;
    char *encoded_str = (char *)dst;
    size_t i = 0, n_result = 0;
    int count;

    while (i < n_bytes)
    {
        count = 1;
        const char cur = buf[i++];
        while (cur == buf[i])
        {
            count++;
            i++;
        }
        encoded_str[n_result++] = count;
        encoded_str[n_result++] = cur;
    }
    return n_result;
}

size_t RleFile2::read(void *dst, size_t max_bytes)
{
    if (!dst)
        return 0;

    char *buf = (char *)dst;
    char count, c;
    size_t n_result = 0;
    while (can_read() && n_result < max_bytes - 1)
    {
        _file->read(&count, 1);
        _file->read(&c, 1);
        for (int j = 0; j < count; j++)
            buf[n_result++] = c;
    }
    return n_result;
}
