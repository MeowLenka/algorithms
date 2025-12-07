#include "helper.h"
#include "library.h"

#include <iostream>
#include <string.h>

void print_library(const Book *library, const int n_book)
{
    int m_title = 8;
    int m_autor = 5;
    printf("\n| №  | Название");
    for (int i = 0; i < n_book; i++)
    {
        m_title = my_len(library[i].title) > m_title ? my_len(library[i].title) + 2 : m_title + 2;
        m_autor = my_len(library[i].autor) > m_autor ? my_len(library[i].autor) : m_autor;
    }
    int total_len = 4 + 1 + m_title + 4 + m_autor + 3 + 4 + 3 + 18;
    print_whitespace(m_title - 6 + 1);
    printf("| Автор");
    print_whitespace(m_autor - 5 + 1);
    printf(" | Год  | Категория         |\n");
    for (int i = 0; i < n_book; i++)
    {
        printf("|%3d.| \"%s\"", i + 1, library[i].title);
        print_whitespace(m_title - my_len(library[i].title) + 1);
        printf("| %s", library[i].autor);
        print_whitespace(m_autor - my_len(library[i].autor) + 1);
        printf(" | %4d | %s", library[i].year, library[i].style);
        print_whitespace(17 - my_len(library[i].style) + 2);
        printf("|\n| ");
        if (my_len(library[i].discription) <= total_len)
            {
                printf("%s", library[i].discription);
                print_whitespace(total_len - my_len(library[i].discription));
                printf(" |\n");
            }
        else
        {
            // for (int j = 0; j < total_len * 2 - 14; j++)
            //     std::cout << library[i].discription[j];
            // printf("... |\n");
            print_utf8_substr(library[i].discription, total_len - 4);
            printf("... |\n");
        }
    }
    printf("\n");
}

void free_space_library(Book *&library, const int n_book, int &size)
{
    if (n_book < size / 2)
    {
        size /= 2;
        Book *new_library = new Book[size];

        for (int i = 0; i < n_book; i++)
        {
            new_library[i] = library[i];
        }
        delete[] library;
        library = new_library;
    }
}

int file_output_library(const Book *library, const char *file_name, const int n_book)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
        return 1;
    fprintf(fp, "%d\n", n_book);
    for (int i = 0; i < n_book; i++)
    {
        fprintf(fp, "%d %s\n", (int)strlen(library[i].title), library[i].title);
        fprintf(fp, "%d %s\n", (int)strlen(library[i].autor), library[i].autor);
        fprintf(fp, "%d %s\n", (int)strlen(library[i].style), library[i].style);
        fprintf(fp, "%d\n", library[i].year);
        fprintf(fp, "%d %s\n", (int)strlen(library[i].discription), library[i].discription);
    }
    fclose(fp);
    return 0;
}

int file_input_library(Book *&library, const char *file_name, int &n_book)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
        return 1;

    int str_size, n;

    fscanf(fp, "%d\n", &n);
    for (int i = n_book; i < n_book + n; i++)
    {

        fscanf(fp, "%d ", &str_size);
        library[i].title = new char[str_size + 1];
        fread(library[i].title, 1, str_size, fp);
        library[i].title[str_size] = '\0';

        fscanf(fp, "%d ", &str_size);
        library[i].autor = new char[str_size + 1];
        fread(library[i].autor, 1, str_size, fp);
        library[i].autor[str_size] = '\0';

        fscanf(fp, "%d ", &str_size);
        library[i].style = new char[str_size + 1];
        fread(library[i].style, 1, str_size, fp);
        library[i].style[str_size] = '\0';

        fscanf(fp, "%d\n", &library[i].year);

        fscanf(fp, "%d ", &str_size);
        library[i].discription = new char[str_size + 1];
        fread(library[i].discription, 1, str_size, fp);
        library[i].discription[str_size] = '\0';
    }
    fclose(fp);
    n_book += n;
    return 0;
}

void year_statistics(const Book *library, const int n_book)
{   
    if (n_book == 0)
    {
        printf("Библиотека пуста. Нет данных для статистики.\n\n");
        return;
    }
    int *years = new int[n_book]; // mediana
    int sum_years = 0;
    int min_year = library[0].year;
    int max_year = library[0].year;
    years[0] = library[0].year;

    for (int i = 1; i < n_book; i++)
    {
        int current_year = library[i].year;
        years[i] = current_year;
        sum_years += current_year;
        
        if (current_year < min_year)
            min_year = current_year;
        if (current_year > max_year)
            max_year = current_year;
    }

    for (int i = 0; i < n_book - 1; i++)
    {
        for (int j = 0; j < n_book - i - 1; j++)
        {
            if (years[j] > years[j + 1])
            {
                int temp = years[j];
                years[j] = years[j + 1];
                years[j + 1] = temp;
            }
        }
    }

    int average_year = sum_years / n_book;
    int median_year;
    if (n_book == 1)
        median_year = years[0];
    else if (n_book % 2 == 0)
        median_year = (years[n_book / 2 - 1] + years[n_book / 2]) / 2;
    else
        median_year = years[n_book / 2];

    printf("\nМинимальный год издания книг: %d\n", min_year);
    printf("Книги с минимальным годом издания:\n");
    for (int i = 0; i < n_book; i++)
    {
        if (library[i].year == min_year)
        {
            printf(" - \"%s\", %s\n", library[i].title, library[i].autor);
        }
    }
    printf("Максимальный год издания книг: %d\n", max_year);
    printf("Книги с максимальным годом издания:\n");
    for (int i = 0; i < n_book; i++)
    {
        if (library[i].year == max_year)
        {
            printf(" - \"%s\", %s\n", library[i].title, library[i].autor);
        }
    }
    printf("Средний арифметический год издания: %d\n", average_year);
    printf("Медианный год издания: %d\n\n", median_year);

}

void sort_library_title(Book *library, const int n_book, int ascending)
{
    for (int i = 0; i < n_book - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n_book - i - 1; j++)
        {
            int comparison = strcasecmp(library[j].title, library[j + 1].title);
            bool should_swap = ascending > 0 ? (comparison > 0) : (comparison < 0);
            if (should_swap)
            {
                Book temp = library[j];
                library[j] = library[j + 1];
                library[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    printf("Сортировка завершена.\n\n");
}

void sort_library_autor(Book *library, const int n_book, int ascending)
{
    for (int i = 0; i < n_book - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n_book - i - 1; j++)
        {
            int comparison = strcasecmp(library[j].autor, library[j + 1].autor);
            bool should_swap = ascending > 0 ? (comparison > 0) : (comparison < 0);
            if (should_swap)
            {
                Book temp = library[j];
                library[j] = library[j + 1];
                library[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    printf("Сортировка завершена.\n\n");
}

void sort_library_year(Book *library, const int n_book, int ascending)
{
    for (int i = 0; i < n_book - 1; i++)
    {
        bool swapped = false;
        for (int j = 0; j < n_book - i - 1; j++)
        {
            int comparison = library[j].year > library[j + 1].year ;
            bool should_swap = ascending > 0 ? (comparison > 0) : (comparison <= 0);
            if (should_swap)
            {
                Book temp = library[j];
                library[j] = library[j + 1];
                library[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    printf("Сортировка завершена.\n\n");
}