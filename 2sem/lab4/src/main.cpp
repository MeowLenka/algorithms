#include <iostream>
#include <cstddef>
#include <fstream>

#include "list.hpp"
#include "other.hpp"


int main()
{
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
