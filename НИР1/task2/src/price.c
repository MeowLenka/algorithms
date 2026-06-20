#include "price.h"
#include <stdio.h>
#include <zlib.h>
#include <stdbool.h> // false, true

unsigned int get_crc32(const PRICE *data, unsigned int count)
{
    uLong crc = crc32(0L, Z_NULL, 0);

    for (unsigned int i = 0; i < count; i++)
    {
        // category hash
        crc = crc32(crc, (const Bytef *)data[i].category, strlen(data[i].category));
        // product name hash
        crc = crc32(crc, (const Bytef *)data[i].good, strlen(data[i].good));
        // price hash
        crc = crc32(crc, (const Bytef *)&data[i].price, sizeof(double));
        // quantity hash
        crc = crc32(crc, (const Bytef *)&data[i].num, sizeof(unsigned int));
    }

    return (unsigned int)crc;
}

DB_HEADER create_header(const char *signature, unsigned int tr_num, unsigned int count, const PRICE *data)
{
    DB_HEADER header;

    strncpy(header.signature, signature, 4);
    header.signature[3] = '\0';

    header.tr_num = tr_num;
    header.st_count = count;
    header.crc32 = get_crc32(data, count);

    return header;
}

void save_database(const char *filename, DB_HEADER *header, PRICE *data, unsigned int count)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        fprintf(stderr, "Error opening file\n");
        return;
    }
    fwrite(header, sizeof(DB_HEADER), 1, file);

    for (unsigned int i = 0; i < count; i++)
    {
        unsigned int len = strlen(data[i].category) + 1;
        fwrite(&len, sizeof(unsigned int), 1, file);
        fwrite(data[i].category, sizeof(char), len, file);

        len = strlen(data[i].good) + 1;
        fwrite(&len, sizeof(unsigned int), 1, file);
        fwrite(data[i].good, sizeof(char), len, file);

        fwrite(&data[i].price, sizeof(double), 1, file);
        fwrite(&data[i].num, sizeof(unsigned int), 1, file);
    }
    fclose(file);
    printf("Database saved to file: %s\n", filename);
    printf("Records count: %u\n", count);
    printf("CRC-32: 0x%08X\n", header->crc32);
}

DB_HEADER load_database(const char *filename, PRICE **data)
{
    DB_HEADER header = {0};
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        fprintf(stderr, "Error opening file\n");
        return header;
    }

    size_t read = fread(&header, sizeof(DB_HEADER), 1, file);
    if (read != 1)
    {
        fprintf(stderr, "Error reading header\n");
        fclose(file);
        return header;
    }

    *data = (PRICE *)malloc(header.st_count * sizeof(PRICE));
    if (!*data)
    {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return header;
    }

    for (unsigned int i = 0; i < header.st_count; i++)
    {
        unsigned int len;
        char *str;

        fread(&len, sizeof(unsigned int), 1, file);
        str = (char *)malloc(len * sizeof(char));
        fread(str, sizeof(char), len, file);
        (*data)[i].category = str;

        fread(&len, sizeof(unsigned int), 1, file);
        str = (char *)malloc(len * sizeof(char));
        fread(str, sizeof(char), len, file);
        (*data)[i].good = str;

        fread(&(*data)[i].price, sizeof(double), 1, file);
        fread(&(*data)[i].num, sizeof(unsigned int), 1, file);
    }

    fclose(file);
    printf("Database loaded from file: %s\n", filename);
    printf("Records count: %u\n", header.st_count);
    printf("Transaction number: %u\n", header.tr_num);
    printf("CRC-32: 0x%08X\n", header.crc32);

    return header;
}

void free_price(PRICE *item)
{
    if (item->category)
    {
        free(item->category);
        item->category = NULL;
    }
    if (item->good)
    {
        free(item->good);
        item->good = NULL;
    }
}

void print_price(const PRICE *item, int index)
{
    printf("Product #%d:\n", index);
    printf("Category: %s\n", item->category);
    printf("Name: %s\n", item->good);
    printf("Price: %.2f rub.\n", item->price);
    printf("Quantity: %u\n", item->num);
    printf("----------------------------\n");
}
