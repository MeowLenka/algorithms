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

void clear_input_buffer();

struct Book
{
    char *autor;
    char *title;
    char *category;
    int year;
    double price;
};

void print_book(const Book &book);
void delete_book(Book &book);

void get_param(Book &book);

void print_max(int *arr, int size);

void print_min(int *arr, int size);

void sort_asc(int *arr, int size);

void sort_desc(int *arr, int size);

double integrate(double (*function)(double), double floor, double ceil, int num);
