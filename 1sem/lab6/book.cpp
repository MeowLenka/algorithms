#include "book.h"
#include "library.h"
#include "helper.h"

#include <iostream>
#include <string.h>

// изменить структуру бук и

void get_param(Book &book)
{
    printf("Введите название книги\n");
    get_string(book.title);

    printf("Введите автора книги\n");
    get_string(book.autor);

    printf("Введите категорию книги\n");
    for (int i = 0; i < 6; i++)
        printf("%d. %s\n", i + 1, categories[i]);
    int num_category;
    while (scanf("%d", &num_category) != 1 || num_category < 1 || num_category > 6)
    {
        printf("Ошибка! Введите корректную категорию!\n");
        clear_input_buffer();
    }
    clear_input_buffer();
    char *category = (char *)categories[num_category - 1];
    book.style = new char[strlen(category) + 1];
    strcpy(book.style, category);

    printf("Введите год издания книги (1400-2025)\n");
    while (scanf("%d", &book.year) != 1 || book.year < 1400 || book.year > 2025)
    {
        printf("Ошибка! Введите корректный год!\n");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("По желанию введите описание книги\n");
    get_string(book.discription);
}

void add_book(Book *&library, const Book &book, int &n_book, int &size)
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
    int title_index = -1;
    for (int i = 0; i < n_book; i++)
    {
        if (strcasecmp(library[i].title, title_to_delete) == 0)
        {
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
        if (n_book == 1)
        {
            free_space_book(library[title_index]);
        }
        else
        {
            for (int i = title_index; i < n_book - 1; i++)
            {
                copy_book(library[i], library[i + 1]);
                free_space_book(library[i + 1]);
            }
        }
        n_book--;
        printf("Книга \"%s\" удалена.\n", title_to_delete);
        return;
    }
}

void copy_book(Book &dest, const Book &src)
{
    if (src.title)
    {
        dest.title = new char[strlen(src.title) + 1];
        strcpy(dest.title, src.title);
    }
    else
    {
        dest.title = nullptr;
    }

    if (src.autor)
    {
        dest.autor = new char[strlen(src.autor) + 1];
        strcpy(dest.autor, src.autor);
    }
    else
    {
        dest.autor = nullptr;
    }

    if (src.style)
    {
        dest.style = new char[strlen(src.style) + 1];
        strcpy(dest.style, src.style);
    }
    else
    {
        dest.style = nullptr;
    }

    if (src.discription)
    {
        dest.discription = new char[strlen(src.discription) + 1];
        strcpy(dest.discription, src.discription);
    }
    else
    {
        dest.discription = nullptr;
    }

    dest.year = src.year;
}

void free_space_book(Book &book)
{
    delete[] book.title;
    delete[] book.autor;
    delete[] book.style;
    delete[] book.discription;

    book.title = nullptr;
    book.autor = nullptr;
    book.style = nullptr;
    book.discription = nullptr;
    book.year = 0;
}

void change_book(Book *&library, char *str_to_change, const int n_book, const int num_search)
{
    int title_index = -1;
    for (int i = 0; i < n_book; i++)
    {
        if (num_search == 1)
        {
            if (strcasecmp(library[i].title, str_to_change) == 0)
            {
                title_index = i;
                break;
            }
        }
        if (num_search == 2)
        {
            if (strcasecmp(library[i].autor, str_to_change) == 0)
            {
                title_index = i;
                break;
            }
        }
        if (num_search == 3)
        {
            if (strcasecmp(library[i].style, str_to_change) == 0)
            {
                title_index = i;
                break;
            }
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
            printf("6. Отменить редактирование\n");
            int choice;
            while (scanf("%d", &choice) != 1)
            {
                printf("Ошибка! Введите число от 1 до 6!\n");
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
                printf("Введите новый год издания книги (1400-2025)\n");
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
                    printf("%d. %s\n", i+1, categories[i]);

                int num_category;
                while (scanf("%d", &num_category) != 1 || num_category < 1 || num_category > 6)
                {
                    printf("Ошибка! Введите корректную категорию!\n");
                    clear_input_buffer();
                }
                clear_input_buffer();
                library[title_index].style = new char[strlen(categories[num_category - 1]) + 1];
                strcpy(library[title_index].style, categories[num_category - 1]);

                break;
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
                printf("Ошибка! Введите число от 1 до 6!\n");
                break;
            }
        }
    }
}

int print_find_book(const Book *library, const int n_book, const char *criteria, const int num_search)
{
    printf("Вот, что удалось найти:\n");
    int found_count = 0;
    for (int i = 0; i < n_book; i++)
    {
        if ((num_search == 1 && strcasecmp(library[i].title, criteria) == 0) ||
            (num_search == 2 && strcasecmp(library[i].autor, criteria) == 0) ||
            (num_search == 3 && strcasecmp(library[i].style, criteria) == 0))
        {
            printf("\n%d. \"%s\"\n", found_count + 1, library[i].title);
            printf("   Автор: %s\n", library[i].autor);
            printf("   Год: %d\n", library[i].year);
            printf("   Категория: %s\n", library[i].style);
            printf("   Описание: %s\n\n", library[i].discription);

            found_count++;
        }
    }
    if (found_count == 0)
    {
        printf("Книг не найдено.\n");
        return 1;
    }
    return 0;
}
