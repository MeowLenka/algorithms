#include "helper.h"

#include <iostream>
#include <string.h>


void clear_input_buffer()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF); // очистка до конца строки
}

void get_string(char *&buffer)
{
    int size = 100, count_symbol = 0;
    buffer = new char[size];
    char symbol = getchar();

    while (symbol != '\n')
    {
        if (count_symbol >= size - 1)
        {
            size *= 1.5;
            char *newBuffer = new char[size];
            for (int i = 0; i < count_symbol; i++)
            {
                newBuffer[i] = buffer[i];
            }
            delete[] buffer;
            buffer = newBuffer;
        }
        buffer[count_symbol] = symbol;
        count_symbol++;
        symbol = getchar();
    }
    buffer[count_symbol] = '\0';
}

unsigned int my_len(const char *str)
{
    unsigned int count = 0;
    while (*str)
    {
        if ((*str & 0xC0) != 0x80)
        {
            count++;
        }
        str++;
    }
    return count;
}

void print_whitespace(const int num)
{
    for (int i = 1; i < num; i++)
        printf(" ");
}
