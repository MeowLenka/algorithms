#include <stdio.h>
/* копирование ввода на вывод; 2-я версия */
int main ()
{
    int c;
    while ((c = getchar()) != EOF)
        putchar(c);
    return 0;
}   