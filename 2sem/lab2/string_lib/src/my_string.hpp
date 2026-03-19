#ifndef MY_STRING_HPP
#define MY_STRING_HPP

class MyString
{
private:
    char *string;

public:
    MyString();
    MyString(const char *str);
    MyString(const MyString& other);

    ~MyString();
    
    
    char get(int i) const;
    void set(int i, char c);
    void set_new_string(const char *str); // замена текущего содержимого на новое
    void print();
    void read_line(); //  замена текущего содержимого строки на строку, считанную с консоли (неопределенного размера)

    int get_str_size() const;
};

#endif
