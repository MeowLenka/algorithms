// Прототипы используемых в данном задании функций:
#include <cmath>

void sort(char *pcFirst, int nNumber, int size,
          void (*Swap)(void *, void *), int (*Compare)(void *, void *));

void swap_int(void *p1, void *p2);
int cmp_int(void *p1, void *p2);

void swap_double(void *p1, void *p2);
int cmp_double(void *p1, void *p2);

void swap_str(void *p1, void *p2);
int cmp_str(void *p1, void *p2);

// Калькулятор
double add(double x, double y);
double sub(double x, double y);
double mul(double x, double y);
double div(double x, double y);
double pow(double, double);

// дедлайн 20-25 минут : чем можно заинтересовать приезжего в россию

struct Book
    {
        char* autor = new char[100];
        char* title = new char[100];
        char* category = new char[100];
        int year;
        double price;
    };

void print_book(const Book& book);
void get_param(const Book &book);