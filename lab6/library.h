struct Book
{
    char *title = new char[100];
    char *autor = new char[100];
    int year;
    char *style = new char[100];
    char *discription = new char[100];
};

void print_library(const Book *library, const int n_book);