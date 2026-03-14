#ifndef BASE_FILE_HPP
#define BASE_FILE_HPP

#include <cstdio>
#include <cstring>

class BaseFile
{
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


// наследование
// кодирование и декодирование

class Base32File : public BaseFile
{
    const char *_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";

    int encoded32_size(size_t raw_size);
    int decoded32_size(size_t encode_size);
    int encode32(const char *raw_data, size_t raw_size, char *dst);
    
    int _decoding_table[256];
    int create_decoding_table();

    int decode32(const char *encoded_data, size_t encoded_size, char *dst);
    int encode32(const char *raw_data, size_t raw_size, char *dst);

public:
    Base32File(const char *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    Base32File(const char *filename, const char *mode, const char *encoding_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    Base32File(FILE *file, const char *encoding_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    ~Base32File();
    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *buf, size_t max_bytes);
};

class RleFile : public BaseFile
{
    size_t encode(const void *src, size_t n_bytes, char *dst);
public:
    RleFile();
    RleFile(const char *filename, const char *mode);
    RleFile(FILE *file);

    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *buf, size_t max_bytes);
    ~RleFile();
};


#endif