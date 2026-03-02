#ifndef TEXT_WRAPPER_HPP
#define TEXT_WRAPPER_HPP

#include "my_string.hpp"

class TextWrapper
{
private:
    const MyString &string;
    int count_prev, count_symbol;
    int stop;
    char *line;
    char *word;
    char symbol;

    void end_of_word();
    void print_roof();
    void print_line(int &count);

    void printc(char c, int n);

public:
    TextWrapper(const MyString &string, int line_width);
    void print_wrapped();
    ~TextWrapper();
};


#endif