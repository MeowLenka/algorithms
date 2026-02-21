#include <iostream>
#include "my_string.hpp"

#ifndef TEXT_WRAPPER_HPP
#define TEXT_WRAPPER_HPP

void printc(char c, int n);

class TextWrapper
{
private:
    const char STOP_STRING = '\n';
    MyString string;
    int count_prev, count_symbol;
    int stop;
    char line[1];
    char word[1];
    char symbol;

    void end_of_word();
    void print_roof();
    void print_line(int &count);

public:
    TextWrapper(const MyString string, int stop);
    void print_wrapped();
};

TextWrapper::TextWrapper(const MyString string, int line_width)
{
    stop = line_width;
    this->string = string;
}

void TextWrapper::print_roof()
{
    printc('-', stop + 4);
    std::cout << "\n";
}

void TextWrapper::print_line(int &count)
{
    std::cout << "| ";
    for (int i = 0; i < count; i++)
        std::cout << line[i];
    printc(' ', stop - count);
    std::cout << " |\n";

    count = 0;
}

void TextWrapper::end_of_word()
{
    if (count_symbol + count_prev >= stop)
    {
        print_line(count_prev);
    }

    for (int i = 0; i < count_symbol; i++)
        line[i + count_prev] = word[i];

    count_prev += count_symbol;
    line[count_prev++] = ' ';
    count_symbol = 0;
}

void TextWrapper::print_wrapped()
{
    line[stop + 1] = {};
    word[stop] = {};
    print_roof();
    for (int i = 0; i < string.get_str_size() + 1; i++)
    {
        symbol = string.get(i);
        if (symbol == ' ')
            end_of_word();

        else
        {
            if (count_symbol >= stop)
            {
                print_line(count_symbol);
            }
            word[count_symbol++] = symbol;
        }
    }
    print_line(count_prev);
    printc(' ', stop - count_prev);
    print_roof();

}

void printc(char c, int n)
{
    for (int i = 0; i < n; i++)
        std::cout << c;
}

#endif