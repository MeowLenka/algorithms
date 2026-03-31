#ifndef MY_STRING_HPP
#define MY_STRING_HPP

class MyString
{
private:
    char *string;

public:
    MyString();
    MyString(const char *str);
    MyString(const MyString &other);
    MyString(MyString &&other) noexcept; // move-конструктор

    const MyString &operator=(const MyString &other);
    MyString &operator=(MyString &&other) noexcept; // move-оператор=

    char &operator[](int index); // оператор индексирования

    MyString &operator+=(const MyString &other);
    MyString &operator+=(const char *str);
    MyString operator+(const MyString &other) const;
    MyString operator+(const char *str) const;
    friend MyString operator+(const char *str, const MyString &mystring);

    bool operator==(const MyString &other) const;
    bool operator==(const char *str) const;
    bool operator!=(const MyString &other) const;
    bool operator!=(const char *str) const;
    bool operator<(const MyString &other) const;
    bool operator<(const char *str) const;
    bool operator<=(const MyString &other) const;
    bool operator<=(const char *str) const;

    friend bool operator==(const char *str, const MyString &mystring);
    friend bool operator!=(const char *str, const MyString &mystring);
    friend bool operator<(const char *str, const MyString &mystring);
    friend bool operator<=(const char *str, const MyString &mystring);

    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);

    ~MyString();

    char get(int i) const;
    void set(int i, char c);
    void set_new_string(const char *str); // замена текущего содержимого на новое
    void print() const;
    void read_line(); //  замена текущего содержимого строки на строку, считанную с консоли (неопределенного размера)

    int get_str_size() const;
};

#endif
