#include "helper.h"
#include "book.h"
#include "library.h"

#include <iostream>
#include <string.h>

void print_library(const Book *library, const int n_book)
{
    int m_title = 8;
    int m_autor = 5;
    printf("| №  | Название");
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
            for (int j = 0; j < total_len * 2 - 10; j++)
                std::cout << library[i].discription[j];
            printf("... |\n");
        }
    }
    printf("\n\n");
}

void free_space_library(const Book *library, const int n_book, int &size)
{
    if (n_book < size / 2) {
        size /= 2;
        Book *new_library = new Book[size];
        
        for (int i = 0; i < n_book; i++) {
            new_library[i] = library[i];
        }
        delete[] library;
        library = new_library;
    }
}