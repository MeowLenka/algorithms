#include <stdbool.h> // false, true

bool palindrome(char *string)
{
    int n = 0;
    char с = string[0];
    while (с != '\0')
        с = string[++n];

    for (int i = 0; i < (int)(n / 2); i++)
    {
        if (string[i] != string[n - i])
            return false;
    }
    return true;
}

bool palindrome_recursive(char *string, int n)
{
    if (n == 0 || n == 1)
        return true;
    if (string[0] == string[n - 1])
        return palindrome_recursive(string, n - 2);
    return false;
}