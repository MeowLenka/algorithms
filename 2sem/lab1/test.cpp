#include <iostream>
#include <math.h>

void print_arr(int a[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << a[i] << ' ';
    }
    std::cout << '\n';
}

void swap(int *m, int *n)
{
    int t = *m;
    *m = *n;
    *n = t;
}

void rotate(int a[], unsigned size, int shift)
{
    for (int k = 0; k < shift; k++)
    {
        for (int i = 0; i < size - 1; i++)
        {
            i = i % size;
            swap(&a[i], &a[i + 1]);
        }
    }
}


int main()
{
    int a[] = {1, 2, 3, 4, 5};
    int size = 5, shift = 2;
    print_arr(a, size);
    rotate(a, size, shift);
    print_arr(a, size);

    return 0;
}
