#include "bool_array.hpp"
#include <cassert>
#include <iostream>

int test_bool_array1()
{
    BoolArray ar1(10);
    assert(ar1.size() == 10);
    for (size_t i = 0; i < 10; i++)
    {
        assert(ar1[i] == false);
    }

    BoolArray ar2(5, true);
    assert(ar2.size() == 5);
    for (size_t i = 0; i < 5; i++)
    {
        assert(ar2[i] == true);
    }

    BoolArray ar3(ar2);
    assert(ar3.size() == 5);
    for (size_t i = 0; i < 5; i++)
    {
        assert(ar3[i] == true);
    }
    ar3[0] = false;
    assert(ar3[0] == false && ar2[0] == true);

    ar2 = ar2;
    assert(ar2[0] == true);

    BoolArray ar(10);
    ar[0] = true;
    ar[3] = true;
    ar[9] = true;

    assert(ar[0] == true && ar[3] == true && ar[9] == true);
    assert(ar[1] == false);

    ar[3] = false;
    assert(ar[3] == false);

    ar[2] = !ar[0] && ar[1];
    assert(ar[2] == false);

    ar[4] = ar[0] || ar[1];
    assert(ar[4] == true);

    return 0;
}

int test_bool_array2()
{
    BoolArray ar(3, true);
    assert(ar.size() == 3);
    assert(ar[0] == true && ar[1] == true && ar[2] == true);

    ar.resize(5, false);
    assert(ar.size() == 5);
    assert(ar[0] == true && ar[1] == true && ar[2] == true);
    assert(ar[3] == false && ar[4] == false);

    ar.resize(2);
    assert(ar.size() == 2);
    assert(ar[0] == true && ar[1] == true);

    return 0;
}

int main()
{
    std::cout << "\nTesting BoolArray...\n";

    int result = 0;

    result = test_bool_array1();
    if (result)
        return result;

    result = test_bool_array2();
    if (result)
        return result;

    std::cout << "\nAll BoolArray tests passed\n";
    return 0;
}