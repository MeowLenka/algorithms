#include <iostream>
#include <cstring>


void printc(char c, int n){
    for (int i=0; i<n; i++)
        std::cout << c;
}


int main()
{
    const char STOP_STRING = '\n';
    int count_prev = 0, count_symbol = 0;
    bool flag = true;
    const int STOP = 40;

    char line[STOP + 1] = {};
    char word[STOP] = {};

    char symbol = getchar();
    while (symbol != STOP_STRING)
    {
        if (flag){
            printc('-', STOP+4);
            std::cout << "\n";
            flag = false;
        }
        if (symbol == ' ')
        {
            if (count_symbol + count_prev >= STOP)
            {
                // std::cout << "pleasse\n";
                std::cout << "| ";
                for (int i = 0; i < count_prev; i++)
                    std::cout << line[i];
                printc(' ', STOP - count_prev);
                std::cout << " |\n";

                count_prev = 0;
            }

            for (int i = 0; i < count_symbol; i++)
                line[i + count_prev] = word[i];

            count_prev += count_symbol;
            line[count_prev++] = ' ';
            count_symbol = 0;
        }
        else
        {
            if (count_symbol >= STOP)
            {
                std::cout << "| ";
                for (int i = 0; i < count_symbol; i++)
                    std::cout << word[i];
                std::cout << " |\n";
                count_symbol = 0;
            }

            word[count_symbol++] = symbol;
        }
        symbol = getchar();
    }
    std::cout << "| ";
    for (int i = 0; i < count_prev; i++)
        std::cout << line[i];
    printc(' ', STOP - count_prev);
    std::cout << " |\n";
    printc('-', STOP+4);
    std::cout << "\n";


    char cat[] = "       \\ \n         /\\_/\\  (\n        ( ^.^ ) _)\n          \\\"/  (\n        ( | | )\n       (__d b__)";
    
    for(int i = 0; cat[i] != '\0'; i++) {
        putchar(cat[i]);
    }

    return 0;
}