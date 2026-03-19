#ifndef BASE_FILE_HPP
#define BASE_FILE_HPP

#include <cstdio>
#include <cstring>

class IFile
{
public:
    virtual bool can_read() = 0;
    virtual bool can_write() = 0;
    virtual size_t write(const void *buf, size_t n_bytes) = 0;
    virtual size_t read(void *buf, size_t max_bytes) = 0;
    virtual ~IFile() {};
};

class BaseFile : public IFile
{
protected:
    FILE *_file;

public:
    BaseFile();
    BaseFile(const char *filename, const char *mode);
    BaseFile(FILE *file);
    virtual ~BaseFile();

    bool is_open();                                    // возвращает true, если файл успешно открыт;
    bool can_read();                                   // возвращает true, если из файла можно считать данные;
    bool can_write();                                  // возвращает true, если в файл можно записать данные;
    size_t write_raw(const void *buf, size_t n_bytes); // записывает указанное количество байт в файл и возвращает количество байт, которое удалось записать;
    size_t read_raw(void *buf, size_t max_bytes);      // читает доступные данные в буфер, но не более указанного количества и возвращает количество байт, которое удалось считать;
    long tell();                                       // возвращает текущий сдвиг файла (см. функцию ftell);
    bool seek(long offset);                            // устанавливает сдвиг файла (см. функцию fseek) и возвращает true, если операция успешна.

    virtual size_t write(const void *buf, size_t n_bytes);
    virtual size_t read(void *buf, size_t max_bytes);
};

// наследование
// кодирование и декодирование

class Base32File : public BaseFile
{
    const char *_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";

    size_t encoded32_size(size_t raw_size);
    size_t decoded32_size(size_t encode_size);

    int _decoding_table[256]{-1};
    int create_decoding_table();

    int decode32(const void *encoded_data, size_t encoded_size, char *dst);
    int encode32(const void *raw_data, size_t raw_size, char *dst);

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
    size_t read(void *dst, size_t max_bytes);
    ~RleFile();
};

class Base32File2 : public IFile
{
    IFile *_file;
    const char *_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";

    size_t encoded32_size(size_t raw_size);
    size_t decoded32_size(size_t encode_size);

    int _decoding_table[256]{-1};
    int create_decoding_table();

    int decode32(const void *encoded_data, size_t encoded_size, char *dst);
    int encode32(const void *raw_data, size_t raw_size, char *dst);

public:
    Base32File2(IFile *file, const char *table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456");
    ~Base32File2();

    bool can_write();
    bool can_read();
    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *buf, size_t max_bytes);
};

////

class RleFile2 : public IFile
{
    IFile *_file;
    size_t encode(const void *src, size_t n_bytes, char *dst);
public:
    RleFile2(IFile *file);
    ~RleFile2();

    bool can_write();
    bool can_read();
    size_t write(const void *buf, size_t n_bytes);
    size_t read(void *dst, size_t max_bytes);
};

#endif