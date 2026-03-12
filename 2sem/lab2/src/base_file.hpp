#ifndef BASE_FILE_HPP
#define BASE_FILE_HPP

#include <cstdio>
#include <cstring>

class BaseFile
{
private:
    FILE *_file;
    char _mode[4];

public:
    BaseFile();
    BaseFile(const char *filename, const char *mode);
    BaseFile(FILE *file);
    ~BaseFile();

    bool is_open();                                    // возвращает true, если файл успешно открыт;
    bool can_read();                                   // возвращает true, если из файла можно считать данные;
    bool can_write();                                  // возвращает true, если в файл можно записать данные;
    size_t write_raw(const void *buf, size_t n_bytes); // записывает указанное количество байт в файл и возвращает количество байт, которое удалось записать;
    size_t read_raw(void *buf, size_t max_bytes);      // читает доступные данные в буфер, но не более указанного количества и возвращает количество байт, которое удалось считать;
    long tell();                                       // возвращает текущий сдвиг файла (см. функцию ftell);
    bool seek(long offset);                            // устанавливает сдвиг файла (см. функцию fseek) и возвращает true, если операция успешна.

    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *buf, size_t max_bytes);
};


BaseFile::BaseFile() : _file(nullptr)
{
    _mode[0] = '\0';
};

BaseFile::BaseFile(const char *filename, const char *mode)
{
    _file = fopen(filename, mode);
    if (!is_open())
    {
        _file = nullptr;
        _mode[0] = '\0';
    }
    else
    {
        strcpy(_mode, mode);
    }
}

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
    if (!_file)
        return false;
    return strchr(_mode, 'r') != nullptr || strchr(_mode, '+') != nullptr;
}

bool BaseFile::can_write()
{
    if (!is_open())
        return false;
    return strchr(_mode, 'w') != nullptr || strchr(_mode, 'a') != nullptr || strchr(_mode, '+') != nullptr;
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

size_t BaseFile::write(const void *buf, size_t n_bytes) {write_raw(const void *buf, size_t n_bytes)};

size_t BaseFile::read(void *buf, size_t max_bytes) {write_raw(void *buf, size_t max_bytes)};

// наследование
// кодирование и декодирование

class Base32File : public BaseFile
{
private:
    char _table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"
    int encoded32_size(int raw_size);
    int decoded32_size(int encode_size);
    int encode32(const char *raw_data, int raw_size, char *dst);

    int decoding_table[256];
    int create_decoding_table();
    // int _ = create_decoding_table();
    int decode32(const char *encoded_data, int encoded_size, char *dst)

public:
    Base32File(char table[] = "\0");
    Base32File(const char *filename, const char *mode, char table[] = "\0");
    Base32File(FILE *file, char table[] = "\0");
    ~Base32File;
    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *buf, size_t max_bytes);
};


Base32File::Base32File(char table[]) : _file(nullptr)
{
    _mode[0] = '\0';
    if (table) : _table = table;
};

Base32File::Base32File(const char *filename, const char *mode, char table[])
{
    _file = fopen(filename, mode);
    if (!is_open())
    {
        _file = nullptr;
        _mode[0] = '\0';
    }
    else
    {
        strcpy(_mode, mode);
    }
    if (table) : _table = table;
}

Base32File::Base32File(FILE *file, char table[]) : _file(file) { if (table) : _table = table; };

Base32File::~Base32File()
{
    fclose(_file);
}

size_t Base32File::write(const void *buf, size_t n_bytes) 
{
    char *dst;
    encode32(buf, encoded32_size(n_bytes), dst)
}

size_t Base32File::read(void *buf, size_t max_bytes)
{
    int _ = create_decoding_table();
    decode32(const char *encoded_data, int encoded_size, char *dst)
}

// 

int Base32File::encoded32_size(int raw_size)
{
    return ceil(raw_size * 8 / 5);
}

int Base32File::decoded32_size(int encode_size)
{
    return encode_size * 5 / 8;
}

// static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";
int Base32File::encode32(const char *raw_data, int raw_size, char *dst)
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
                dst[(count / 5) - 1] = table[buf];
                buf = 0;
            }
        }
    }
    if (count % 5)
    {
        dst[(count / 5)] = table[buf];
    }

    return 0;
}

int Base32File::create_decoding_table()
{
    for (int i = 0; i < 256; i++)
    {
        if (i >= 'A' && i <= 'Z')
        {
            decoding_table[i] = i - 'A';
        }
        else if (i >= '1' && i <= '9')
        {
            decoding_table[i] = 'Z' - 'A' + 1 + i - '1';
        }
        else
        {
            decoding_table[i] = -1;
        }
    }
    return 0;
}

int Base32File::decode32(const char *encoded_data, int encoded_size, char *dst)
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
        if (decoding_table[letter] == -1)
            return 2;
        for (int j = 4; j >= 0; j--)
        {
            buf += ((decoding_table[letter] >> j) & 1) << (7 - (count++ % 8));
            if (count % 8 == 0)
            {
                dst[(count / 8) - 1] = buf;
                buf = 0;
            }
        }
    }
    return 0;
}

/// RLE

class RleFile : public BaseFile
{
private:
    FILE *_file;
    char _mode[4];

public:
    RleFile(const char *filename, const char *mode);
    void write(const char *input);
};

void RleFile::write(const char *buf)
{
    int buf_len = strlen(buf);
    int output_index = 0;
    int count;
    char current_char;

    char *output;

    for (int i = 0; i < buf_len; i++)
    {
        current_char = buf[i];
        count = 1;

        while (i + 1 < buf_len && buf[i] == buf[i + 1])
        {
            count++;
            i++;
        }

        if (count > 1)
        {
            int written = snprintf(output + output_index,
                                   (buf_len * 3 + 1) - output_index,
                                   "%d%c", count, current_char);
            if (written < 0)
                break;
            output_index += written;
        }
        else
        {
            if (output_index < buf_len * 3)
            {
                output[output_index++] = current_char;
            }
            else
            {
                break;
            }
        }
    }
    output[output_index] = '\0';

    write_raw(output, count);
}

RleFile::RleFile(const char *filename, const char *mode)
{
    _file = fopen(filename, mode);
    if (!is_open())
    {
        _file = nullptr;
        _mode[0] = '\0';
    }
    else
    {
        strcpy(_mode, mode);
    }
}

#endif