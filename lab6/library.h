#ifndef LIBRARY_H
#define LIBRARY_H

void print_library(const Book *library, const int n_book);
void free_space_library(const Book *library, const int n_book, int &size);

#endif