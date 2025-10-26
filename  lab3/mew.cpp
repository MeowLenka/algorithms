// ввод неогрониченного числа символов
#include <iostream>


int main()
{
    int size = 10, count_symbol = 0;
    char *buffer = new char[size];
    
    char symbol = getchar();

    while (symbol != '\n')
    { 
        if (count_symbol >= size)
        {
            size *= 2;
            char *newBuffer = new char[size];
            for (int i = 0; i < count_symbol; i++){
                newBuffer[i] = buffer[i];
            }
            delete [] buffer;
            buffer = newBuffer; 
        }
        buffer[count_symbol++] = symbol;
        symbol = getchar();
    }

    for (int i=0; i < count_symbol; i++){
        std::cout << buffer[i];
    }
    delete [] buffer;
}