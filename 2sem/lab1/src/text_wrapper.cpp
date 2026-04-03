#include <iostream>
#include "text_wrapper.hpp"

TextWrapper::TextWrapper(const MyString &string, int line_width) : string(string), stop(line_width)
{
private:
    MyString string;
    int count_prev = 0, count_symbol = 0;
    int stop;
    char *line;
    char *word;
    char symbol;

    void end_of_word();
    void print_roof();
    void print_line(int &count);

public:
    TextWrapper(const MyString string, int stop);
    void print_wrapped();
    ~TextWrapper();
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
    line = new char[stop + 1];
    word = new char[stop];

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
    print_roof();
}

TextWrapper::~TextWrapper()
{
    delete[] line;
    delete[] word;
}

void TextWrapper::printc(char c, int n)
{
    for (int i = 0; i < n; i++)
        std::cout << c;
}