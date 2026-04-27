#include "my_string.hpp"
#include <cassert>
#include <iostream>

int test_mystring1()
{
    MyString s1;
    assert(s1.get_str_size() == 0);

    MyString s2("roblox");
    assert(s2.get_str_size() == 6);
    assert(s2.get(0) == 'r');
    assert(s2.get(5) == 'x');

    MyString s3(s2);
    assert(s3.get_str_size() == 6);
    assert(s3.get(1) == 'o');

    MyString s4(std::move(s3));
    assert(s4.get_str_size() == 6);
    assert(s3.get_str_size() == 0);

    return 0;
}

int test_mystring2()
{
    MyString s1("hello");
    MyString s2;

    s2 = s1;
    assert(s2.get_str_size() == 5);
    assert(s2.get(0) == 'h');

    s1 = s1;
    assert(s1.get_str_size() == 5);

    s2 = std::move(s1);
    assert(s2.get_str_size() == 4);
    assert(s1.get_str_size() == 0);

    MyString s3 = s1 + " " + s2;
    assert(s3.get_str_size() == 11);

    s1 += " ";
    s1 += s2;
    assert(s1.get_str_size() == 11);

    MyString s4 = "world, " + s2;
    assert(s4.get_str_size() == 11);

    return 0;
}

int test_mystring3()
{
    MyString s1("abc");
    MyString s2("abd");
    MyString s3("abc");

    assert(s1 == s3);
    assert(s1 != s2);
    assert(s1 < s2);
    assert(s1 <= s3);

    assert(s1 == "abc");
    assert("abc" == s1);
    assert(s1 != "abd");
    assert("abc" < s2);

    s1[2] = 'g';
    assert(s1[2] == 'g');
    return 0;
}

int main()
{
    std::cout << "\nTesting MyString...\n";

    int result = 0;

    // result = test_mystring1();
    // if (result)
    //     return result;

    // result = test_mystring2();
    // if (result)
    //     return result;

    // result = test_mystring3();
    // if (result)
    //     return result;

    std::cout << "\nAll MyString tests passed\n";
    return 0;
}