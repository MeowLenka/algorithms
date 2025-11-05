#include <iostream>

int main(){
    int x = 1;
    int y = -1;
    int &px = x;
    int &py = y;
    
    int c = x;
    x = y;
    y = c;

    int k = 0;
    std::cout << x << ' ' << y;

    return 0;
}

