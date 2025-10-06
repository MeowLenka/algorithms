#include <iostream>

int main()
{
    char x = 0xff;
    unsigned char y = 0xff;
    std::cout << (x < y) << "\n";
    std::cout << (int) x;
}