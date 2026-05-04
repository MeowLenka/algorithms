#include <iostream>
#include <cstddef>
#include <fstream>

#include "list.hpp"
#include "other.hpp"

// explicit_lib - явный
#include "circle_func.hpp"

// implicit_lib - неявный
#ifdef __APPLE__
#include <dlfcn.h>
#endif

typedef void (*CatPrintFunc)(const char*);

int main()
{

    // неявное связывание
    double radius = 5.0;
    std::cout << "Radius: " << radius << "\n";
    std::cout << "Square: " << CircleFunc::circle_square(radius) << "\n";
    std::cout << "Perimeter: " << CircleFunc::circle_perimeter(radius) << "\n\n";


    // явное связывание
    #ifdef __APPLE__
    const char* libPath = "./libexplicit_lib.dylib";
    #else
    const char* libPath = "./libexplicit_lib.so";
    #endif
    
    void* handle = dlopen(libPath, RTLD_LAZY);
    if (!handle) {
        std::cerr << "Error: " << dlerror() << '\n';
        return 1;
    }
    
    CatPrintFunc print_cat = (CatPrintFunc)dlsym(handle, "print_cat");
    const char* error = dlerror();
    if (error) {
        std::cerr << "Error: " << error << '\n';
        dlclose(handle);
        return 1;
    }
    
    print_cat("Looooong message, extra task for automatic weapon xexexexe");
    dlclose(handle);
    
    // 
    std::cout << "Continue....\n";
    int k = 5;
    List myList;
    Point center(5, 5);

    for (int i = 1; i <= 5; i++)
    {
        Circle c(i * 2, center);
        myList.add_end(c);
    }
    std::cout << myList;
    std::cout << "-------------------------\n";

    Circle target(6, center);
    myList.remove(target);
    std::cout << myList;
    std::cout << "-------------------------\n";

    Circle another(2, center);
    myList.add_begin(another);
    myList.add_end(another);

    std::cout << myList;
    std::cout << "-------------------------\n";

    int n = myList.remove_all(another);
    std::cout << "Was deleted: " << n << " circles\n";
    std::cout << myList;
    std::cout << "-------------------------\n";


    List list;
    Point p1(10, 12);
    Point p3(10, 12);
    Circle c1(5.0, p1);
    Circle c2(1.0);
    Circle c3(3.0, p3);
    Circle c4(2.0);
    Circle c5(4.0, p1);

    list.add_end(c1);
    list.add_end(c2);
    list.add_end(c3);
    list.add_end(c4);
    list.add_end(c5);

    // list.print();
    std::cout << list;
    list.sort(); 
    std::cout << "-------------------------\n";
    std::cout << list;
    std::cout << "-------------------------\n";

    char filename1[80];
    std::cout << "Output file name: ";
    std::cin >> filename1;
    list.save_to_file(filename1);
    std::cout << "List saved to " << filename1 << '\n';

    List new_list;
    char filename2[80];
    std::cout << "Input file name: ";
    std::cin >> filename2;

    new_list.load_from_file(filename2);
    std::cout << "Loaded list:\n";
    std::cout << new_list;

    return 0;
}

// in build/
// make explicit_lib 
// cmake .. 
// make
// ./lab4