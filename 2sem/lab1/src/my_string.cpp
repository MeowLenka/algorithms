#include <iostream>
#include "my_string.hpp"

MyString::MyString()
{
    string = new char[1];
}

MyString::MyString(const char *str)
{
    string = new char[strlen(str) + 1]; // +1 для '\0'
    strcpy(string, str);
}

MyString::MyString(const MyString& other)
{
    string = new char[strlen(other.string) + 1]; // +1 для '\0'
    strcpy(string, other.string);
}

char MyString::get(int i) const
{
    return string[i];
}

void MyString::set(int i, char c)
{
    string[i] = c;
}

void MyString::set_new_string(const char *str)
{
    delete[] string;
    string = new char[strlen(str) + 1];
    strcpy(string, str);
}

void MyString::print()
{
    for (int i = 0; i < strlen(string); i++)
        std::cout << string[i];
}

void MyString::read_line()
{
    int size = 10, i = 0;
    delete[] string;
    string = new char[size];

    char let = getchar();
    while (let != '\n' && let != EOF)
    {
        if (i >= size)
        {
            char *copy = new char[size * 2];
            for (int j = 0; j < size; j++)
            {
                copy[j] = string[j];
            }
            delete[] string;
            string = copy;
            size *= 2;
        }
        string[i++] = let;
        let = getchar();
    }
}

MyString::~MyString()
{
    delete[] string;
}

int MyString::get_str_size() const
{
    return strlen(string);
}