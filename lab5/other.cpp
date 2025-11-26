#include "other.h"
#include <string.h>
#include <iostream>

void sort(char *pcFirst, int nNumber, int size,
          void (*Swap)(void *, void *), int (*Compare)(void *, void *))
{
    int i;
    for (i = 1; i < nNumber; i++)
        for (int j = nNumber - 1; j >= i; j--)
        {
            char *pCurrent = pcFirst + j * size;
            char *pPrevious = pcFirst + (j - 1) * size;
            if ((*Compare)(pPrevious, pCurrent) > 0) // требуется
                                                     // переставить
                (*Swap)(pPrevious, pCurrent);
        }
}

void swap_int(void *p1, void *p2)
{
    int *pp1 = (int *)p1;
    int *pp2 = (int *)p2;
    int p = *pp1;
    *pp1 = *pp2;
    *pp2 = p;
}

int cmp_int(void *p1, void *p2)
{
    int nResult;
    int *pp1 = (int *)p1;
    int *pp2 = (int *)p2;
    if (*pp1 < *pp2)
    {
        nResult = -1;
    }
    else if (*pp1 > *pp2)
    {
        nResult = 1;
    }
    else
    {
        nResult = 0;
    }
    return nResult;
}

void swap_double(void *p1, void *p2)
{
    double *pp1 = (double *)p1;
    double *pp2 = (double *)p2;
    double p = *pp1;
    *pp1 = *pp2;
    *pp2 = p;
}

int cmp_double(void *p1, void *p2)
{
    double nResult;
    double *pp1 = (double *)p1;
    double *pp2 = (double *)p2;
    if (*pp1 < *pp2)
    {
        nResult = -1;
    }
    else if (*pp1 > *pp2)
    {
        nResult = 1;
    }
    else
    {
        nResult = 0;
    }
    return nResult;
}

void swap_str(void *p1, void *p2)
{
    char **pp1 = (char **)p1;
    char **pp2 = (char **)p2;
    std::cout << *pp1 << ' ' << *pp2 << '\n';
    char *p = *pp1;
    *pp1 = *pp2;
    *pp2 = p;
}

int cmp_str(void *p1, void *p2)
{
    char nResult;
    char **pp1 = (char **)p1;
    char **pp2 = (char **)p2;
    return strcmp(*pp1, *pp2);
}

// Калькулятор
double add(double x, double y)
{
    return x + y;
}

double sub(double x, double y)
{
    return x - y;
}

double mul(double x, double y)
{
    return x * y;
}

double div(double x, double y)
{
    if (y == 0)
    {
        return 0;
    }
    return x / y;
}

void print_book(const Book &book)
{
    printf("Автор:     %s\n", book.autor);
    printf("Название:  %s\n", book.title);
    printf("Категория: %s\n", book.category);
    printf("Год:       %d\n", book.year);
    printf("Цена:      %.3f руб.\n", book.price);
}

const char *categories[] = {"fantasy", "detective", "comedy", "horror", "science-fiction", "mystery"};

void clear_input_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF); // очистка до конца строки
}

void get_param(const Book &book)
{
    printf("Введите название книги\n");
    scanf("%99[^\n]", book.title); // читаем всю строку до \n
    clear_input_buffer();

    printf("Введите автора книги\n");
    scanf("%99[^\n]", book.autor);
    clear_input_buffer();

    printf("Введите категорию книги\n");
    for (int i = 0; i < 6; i++)
    {
        printf("-> %s\n", categories[i]);
    }

    bool correct_category = false;
    while (!correct_category)
    {
        scanf("%99[^\n]", book.category);
        clear_input_buffer();
        for (int i = 0; i < 6; i++)
        {   
            if (strcmp(categories[i], book.category) == 0)
            {
                correct_category = true;
                break;
            }
        }
        if (!correct_category)
            {
                printf("Ошибка! Введите корректную категорию!\n");
            }
    }

    printf("Введите год издания книги\n");
    while (scanf("%d", &book.year) != 1 || book.year <= 0 || book.year > 2025)
    {
        if (book.price)
            printf("Ошибка! Введите корректный год!\n");
        while (getchar() != '\n');
    }
    getchar();

    printf("Введите цену книги\n");
    while (scanf("%lf", &book.price) != 1 || book.price < 0)
    {
        printf("Ошибка! Введите корректную цену!\n");
        while (getchar() != '\n');
    }
    getchar();
}