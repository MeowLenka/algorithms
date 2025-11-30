#include "book.h"
#include "library.h"
#include "helper.h"

#include <iostream>
#include <string.h>


const char *categories[] = {"фантастика", "детектив", "комедия", "ужасы", "приключения", "научно-популарное"};

void get_param(Book &book)
{
    printf("Введите название книги\n");
    get_string(book.title);

    printf("Введите автора книги\n");
    get_string(book.autor);

    printf("Введите категорию книги\n");
    for (int i = 0; i < 6; i++)
        printf("-> %s\n", categories[i]);


    bool correct_style = false;
    while (!correct_style) {
        char temp_style[100];
        printf("Выберите категорию: ");
        scanf("%99[^\n]", temp_style);
        clear_input_buffer();
        
        for (int i = 0; i < 6; i++) {
            if (strcmp(categories[i], temp_style) == 0) {
                correct_style = true;
                delete[] book.style;
                book.style = new char[strlen(temp_style) + 1];
                strcpy(book.style, temp_style);
                break;
            }
        }
        if (!correct_style) {
            printf("Ошибка! Введите корректную категорию!\n");
        }
    }

    printf("Введите год издания книги\n");
    while (scanf("%d", &book.year) != 1 || book.year < 0 || book.year > 2025)
    {
        printf("Ошибка! Введите корректный год!\n");
        while (getchar() != '\n');
    }
    clear_input_buffer();

    printf("По желанию введите описание книги\n");
    get_string(book.discription);
}

void add_book(Book *library, const Book &book, int &n_book, int &size)
{
    n_book++;
    if (n_book > size)
    {
        size *= 2;
        struct Book *temp = new struct Book[size];
        for (int i = 0; i < size; i++)
        {
            temp[i] = library[i];
        }
        delete[] library;
        library = temp;
    }
    library[n_book - 1] = book;
}