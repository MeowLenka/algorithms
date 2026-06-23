#ifndef PRICE_H
#define PRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <zlib.h>    // CRC-32


typedef struct
{
    char *category;
    char *good;
    double price;
    unsigned int num;
} PRICE;

typedef struct 
{
    char signature[4];
    unsigned int tr_num;  
    unsigned int st_count;   
    unsigned int crc32;  
} DB_HEADER;

DB_HEADER create_header(const char *signature, unsigned int tx_num, unsigned int count, const PRICE *data);
unsigned int get_crc32(const PRICE *data, unsigned int count);
void save_database(const char *filename, DB_HEADER *header, PRICE *data, unsigned int count);
DB_HEADER load_database(const char *filename, PRICE **data);
void free_price(PRICE *item);


#endif