#include <iostream>
#include "my_string.hpp"

MyString::MyString()
{
    string = new char[1];
    string[0] = 0;
}

MyString::MyString(const char *str)
{
    string = new char[strlen(str) + 1]; // +1 для '\0'
    strcpy(string, str);
}

MyString::MyString(const MyString &other)
{
    string = new char[strlen(other.string) + 1]; // +1 для '\0'
    strcpy(string, other.string);
}

MyString::MyString(MyString &&other) noexcept : string(other.string)
{
    other.string = nullptr;
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

void MyString::print() const
{
    for (int i = 0; i < strlen(string); i++)
        std::cout << string[i];
    std::cout << '\n';
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

const MyString &MyString::operator=(const MyString &other)
{
    if (this != &other)
    {
        delete[] string;
        string = new char[strlen(other.string) + 1]; // +1 для '\0'
        strcpy(string, other.string);
        // std::cout << "operator=string\n";
    }
    return *this;
}

MyString &MyString::operator=(MyString &&other) noexcept
{
    if (this != &other)
    {
        delete[] string;
        string = other.string;

        other.string = nullptr;
    }
    return *this;
}

MyString MyString::operator+(const MyString &other) const
{
    char *temp = new char[strlen(string) + strlen(other.string) + 1];
    strcpy(temp, string);
    strcat(temp, other.string);
    MyString dst(temp);
    delete[] temp;
    return dst;
}

MyString MyString::operator+(const char *str) const
{
    char *temp = new char[strlen(string) + strlen(str) + 1];
    strcpy(temp, string);
    strcat(temp, str);
    MyString dst(temp);
    delete[] temp;
    return dst;
}

MyString &MyString::operator+=(const MyString &other)
{
    char *temp = new char[strlen(string) + strlen(other.string) + 1];
    strcat(temp, string);
    strcat(temp, other.string);

    delete[] string;
    string = new char[strlen(temp) + 1];
    strcpy(string, temp);
    delete[] temp;
    return *this;
}

MyString &MyString::operator+=(const char *str)
{
    char *temp = new char[strlen(string) + strlen(str) + 1];
    strcat(temp, string);
    strcat(temp, str);

    delete[] string;
    string = new char[strlen(temp) + 1];
    strcpy(string, temp);
    delete[] temp;
    return *this;
}

bool MyString::operator==(const MyString &other) const
{
    if (strcmp(string, other.string) == 0)
        return true;
    return false;
}

bool MyString::operator==(const char *str) const
{
    if (strcmp(string, str) == 0)
        return true;
    return false;
}

bool MyString::operator!=(const MyString &other) const
{
    if (strcmp(string, other.string) != 0)
        return true;
    return false;
}

bool MyString::operator!=(const char *str) const
{
    if (strcmp(string, str) != 0)
        return true;
    return false;
}

bool MyString::operator<(const MyString &other) const
{
    if (strcmp(string, other.string) < 0)
        return true;
    return false;
}

bool MyString::operator<(const char *str) const
{
    if (strcmp(string, str) < 0)
        return true;
    return false;
}

bool MyString::operator<=(const MyString &other) const
{
    if (strcmp(string, other.string) <= 0)
        return true;
    return false;
}

bool MyString::operator<=(const char *str) const
{
    if (strcmp(string, str) <= 0)
        return true;
    return false;
}

MyString operator+(const char *str, const MyString &mystring)
{
    char *temp = new char[strlen(mystring.string) + strlen(str) + 1];
    strcpy(temp, str);
    strcat(temp, mystring.string);
    MyString dst(temp);
    delete[] temp;
    return dst;
}

bool operator==(const char *str, const MyString &mystring)
{
    if (strcmp(mystring.string, str) == 0)
        return true;
    return false;
}

bool operator!=(const char *str, const MyString &mystring)
{
    if (strcmp(mystring.string, str) != 0)
        return true;
    return false;
}

bool operator<(const char *str, const MyString &mystring)
{
    if (strcmp(str, mystring.string) < 0)
        return true;
    return false;
}
bool operator<=(const char *str, const MyString &mystring)
{
    if (strcmp(str, mystring.string) <= 0)
        return true;
    return false;
}

char &MyString::operator[](int index)
{
    if (index >= strlen(string))
        throw "Array index out of bounds exception";
    return string[index];
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    if (str.string != nullptr)
    {
        os << str.string;
    }
    return os;
}

std::istream& operator>>(std::istream& is, MyString& str)
{
    str.read_line();  
    return is;
}