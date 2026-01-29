#include "other.h"

#include <iostream>
#include <cstring>
#include <cstdarg>
#include <cassert>

int inc_by_value(int val)
{
    return ++val;
}

void inc_by_pointer(int *val)
{
    ++(*val);
}

void inc_by_reference(int &val)
{
    ++val;
}

void swap(int *x, int *y)
{
    int c = *x;
    *x = *y;
    *y = c;
}

void swap(int &x, int &y)
{
    int c = x;
    x = y;
    y = c;
}

// 3
int minArrOne(const int arr[], const int size)
{
    assert(size > 0);
    int min = 10000;
    for (int i = 0; i < size; i++)
    {
        min = arr[i] < min ? arr[i] : min;
    }
    return min;
}

// указатель на константный указатель на константный int
int minArrTwo(const int *const *arr, int n, int m)
{
    int min = 10000;
    for (int i = 0; i < n; i++)
    {
        int cur = minArrOne(arr[i], m);
        min = cur < min ? cur : min;
    }
    return min;
}

int my_str_cmp(const char *str1, const char *str2)
{
    // return strcmp(str1, str2);
    int i = 0;
    while (str1[i] != '\0' || str2[i] != '\0')
    {
        if (str1[i] > str2[i])
        {
            return 1;
        }
        else if (str1[i] < str2[i])
        {
            return -1;
        }
        i++;
    }
    if (str1[i] == str2[i] && str2[i] == '\0')
    {
        return 0;
    }
    if (str1[i] == '\0')
    {
        return -1;
    }
    if (str2[i] == '\0')
    {
        return 1;
    }
}

// 4
int is_a_leap(int year)
{
    if ((year % 400 == 0 || year % 4 == 0) && year % 100 != 0)
    {
        return 1;
    }
    return 0;
}

int day_of_year(int day, int month, int year, int nDayTab[2][12])
{
    int result = 0;
    int leap = is_a_leap(year);
    for (int i = 0; i < (month - 1); i++)
    {
        result += nDayTab[leap][i];
    }
    result += day;
    return result;
}

int day_of_month(int *day, int year, int nDayTab[2][12])
{
    int month = 1;
    int leap = is_a_leap(year);

    for (int i = 0; i < 12; i++)
    {
        if (*day > nDayTab[leap][i])
        {
            *day -= nDayTab[leap][i];
            month++;
        }
        else
        {
            return month;
        }
    }
}

// 5
void print_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}

void add_unique(int *&arr, int *size, int newValue)
{
    bool inArr = false;
    for (int i = 0; i < *size; i++)
    {
        if (newValue == arr[i])
        {
            inArr = true;
        }
    }
    if (!inArr)
    {
        int *newArr = new int[*size + 1];
        for (int i = 0; i < *size; i++)
        {
            newArr[i] = arr[i];
        }
        newArr[(*size)++] = newValue;

        delete[] arr;
        arr = newArr;
    }
}

// 6
int recur_summ(int n)
{
    if (n == 1)
        return 1;

    return n + recur_summ(n - 1);
}

// 7
int encoded32_size(int raw_size)
{
    return ceil(raw_size * 8 / 5);
}

int decoded32_size(int encode_size)
{
    return encode_size * 5 / 8;
}

static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";
int encode32(const char *raw_data, int raw_size, char *dst)
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
static int decoding_table[256];
int create_decoding_table(){
    for(int i = 0; i < 256; i++){
        if (i >= 'A' && i <= 'Z'){
            decoding_table[i] = i - 'A';
        }
        else if (i >= '1' && i <= '9'){
            decoding_table[i] = 'Z' - 'A' + 1 + i - '1';
        }else{
            decoding_table[i] = -1;
        }
    }
    return 0;
}
static int _ = create_decoding_table();

int decode32(const char *encoded_data, int encoded_size, char *dst)
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
        if(decoding_table[letter] == -1)
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

// 8
void var_args(int n, ...) // вариадические параметры
{

    va_list arg;
    va_start(arg, n);
    int val = n;
    while (val != 0)
    {
        std::cout << val << ' ';
        val = va_arg(arg, int);
    }
    std::cout << '\n';
    va_end(arg);
}

// 9
int *my_min(int arr[], int size)
{
    int *pmin = &arr[0];

    for (int i = 1; i < size; i++)
    {
        if (arr[i] < *pmin)
        {
            pmin = &arr[i];
        }
    }
    return pmin;
}