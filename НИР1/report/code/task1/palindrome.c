#include <stdbool.h> // false, true

bool palindrome(char *string, int n)
{
    for (int i = 0; i < (int)(n / 2); i++)
    {
        if (string[i] != string[n - 1 - i])
            return false;
    }
    return true;
}

bool palindrome_recursive(char *string, int n)
{
    if (n == 0 || n == 1)
        return true;
    if (string[0] == string[n - 1])
        return palindrome_recursive(string + 1, n - 2);
    return false;
}

int string_len(char *string)
{
    int n = 0;
    char c = string[0];
    while (c != '\0')
        c = string[++n];
    return n;
}