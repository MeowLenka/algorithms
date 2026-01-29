#ifndef LIBRARY_H
#define LIBRARY_H

#include "book.h"

void print_library(const Book *library, const int n_book);
void free_space_library(Book *&library, const int n_book, int &size);

int file_output_library(const Book *library, const char *file_name, const int n_book);
int file_input_library(Book *&library, const char *file_name, int &n_book);

void year_statistics(const Book *library, const int n_book);

void sort_library_title(Book *library, const int n_book, int ascending);
void sort_library_autor(Book *library, const int n_book, int ascending);
void sort_library_year(Book *library, const int n_book, int ascending);

#endif