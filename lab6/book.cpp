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
    while (!correct_style)
    {
        char temp_style[100];
        printf("Выберите категорию: ");
        scanf("%99[^\n]", temp_style);
        clear_input_buffer();

        for (int i = 0; i < 6; i++)
        {
            if (strcmp(categories[i], temp_style) == 0)
            {
                correct_style = true;
                delete[] book.style;
                book.style = new char[my_len(temp_style) + 1];
                strcpy(book.style, temp_style);
                break;
            }
        }
        if (!correct_style)
        {
            printf("Ошибка! Введите корректную категорию!\n");
        }
    }

    printf("Введите год издания книги\n");
    while (scanf("%d", &book.year) != 1 || book.year < 1400 || book.year > 2025)
    {
        printf("Ошибка! Введите корректный год!\n");
        clear_input_buffer();
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

void delete_book(Book *library, char *title_to_delete, int &n_book)
{
    if (!n_book)
    {
        printf("Библиотека пуста.\n");
        return;
    }
    int title_index = -1;
    for (int i = 0; i < 6; i++)
    {
        if (strcasecmp(library[i].title, title_to_delete) == 0)
        {
            library[i];
            title_index = i;
            break;
        }
    }
    if (title_index == -1)
    {
        printf("Такой книги не существует.\n");
        return;
    }
    else
    {
        for (int i = title_index; i < n_book - 1; i++)
        {
            library[i] = library[i + 1];
        }
        n_book--;
        printf("Книга \"%s\" удалена.\n", title_to_delete);
        return;
    }
}

void change_book(Book *library, char *title_to_delete, const int n_book)
{
    if (!n_book)
    {
        printf("Библиотека пуста.\n");
        return;
    }
    int title_index = -1;
    for (int i = 0; i < 6; i++)
    {
        if (strcasecmp(library[i].title, title_to_delete) == 0)
        {
            library[i];
            title_index = i;
            break;
        }
    }
    if (title_index == -1)
    {
        printf("Такой книги не существует.\n");
        return;
    }
    else
    {
        bool changing = true;
        while (changing)
        {

            printf("Что вы хотите изменить?\n");
            printf("1. Название\n");
            printf("2. Автора\n");
            printf("3. Год\n");
            printf("4. Категорию\n");
            printf("5. Описание\n");
            int choice;
            while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4)
            {
                printf("Ошибка! Введите число от 1 до 4!\n");
                clear_input_buffer();
            }
            clear_input_buffer();
            switch (choice)
            {
            case 1:
                printf("Введите новое название книги\n");
                delete[] library[title_index].title;
                get_string(library[title_index].title);
                break;
            case 2:
                printf("Введите нового автора книги\n");
                delete[] library[title_index].autor;
                get_string(library[title_index].autor);
                break;
            case 3:
                printf("Введите новый год издания книги\n");
                while (scanf("%d", &library[title_index].year) != 1 || library[title_index].year < 1400 || library[title_index].year > 2025)
                {
                    printf("Ошибка! Введите корректный год!\n");
                    clear_input_buffer();
                }
                clear_input_buffer();
                break;
            case 4:
                printf("Введите новую категорию книги\n");
                for (int i = 0; i < 6; i++)
                    printf("-> %s\n", categories[i]);

                bool correct_style = false;
                while (!correct_style)
                {
                    char temp_style[100];
                    printf("Выберите категорию: ");
                    scanf("%99[^\n]", temp_style);
                    clear_input_buffer();

                    for (int i = 0; i < 6; i++)
                    {
                        if (strcmp(categories[i], temp_style) == 0)
                        {
                            correct_style = true;
                            delete[] library[title_index].style;
                            library[title_index].style = new char[my_len(temp_style) + 1];
                            strcpy(library[title_index].style, temp_style);
                            break;
                        }
                    }
                    if (!correct_style)
                    {
                        printf("Ошибка! Введите корректную категорию!\n");
                    }
                }
            case 5:
                printf("Введите новое описание книги\n");
                delete[] library[title_index].discription;
                get_string(library[title_index].discription);
                break;
            case 6:
                changing = false;
                printf("Изменение книги завершено.\n");
                break;
            default:
                printf("Ошибка! Введите число от 1 до 4!\n");
                break;
            }
        }
    }
}