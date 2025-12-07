#ifndef BOOK_H
#define BOOK_H

struct Book
{
    char *title;
    char *autor;
    int year;
    char *style;
    char *discription;
};

static const char *categories[] = {"фантастика", "детектив", "комедия", "ужасы", "приключения", "научно-популярное"};


void get_param(Book &book);

void add_book(Book *&library, const Book &book, int &n_book, int &size);

void delete_book(Book *library, char *title_to_delete, int &n_book);

void free_space_book(Book &book);

void copy_book(Book &dest, const Book &src);

void change_book(Book *&library, char *str_to_change, const int n_book, const int num_search);
int print_find_book(const Book *library, const int n_book, const char *criteria, const int num_search);

#endif