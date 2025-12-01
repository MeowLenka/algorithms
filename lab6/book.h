#ifndef BOOK_H
#define BOOK_H

#include "library.h"

struct Book
{
    char *title;
    char *autor;
    int year;
    char *style;
    char *discription;
};

void get_param(Book &book);

void add_book(Book* library, const Book &book, int &n_book, int &size);

void delete_book(Book *library, char *title_to_delete, int &n_book);

#endif