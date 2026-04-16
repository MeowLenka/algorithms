#include "matrix.hpp"
#include <cassert>
#include <iostream>

int test_matrix1()
{
    Matrix m1(3);
    assert(m1.get_height() == 3 && m1.get_width() == 3);
    assert(m1.get(0, 1) == 0);
    assert(m1.get(1, 1) == 1);

    Matrix m2(2, 2, 5);
    assert(m2.get_height() == 2 && m2.get_width() == 2);
    assert(m2.get(2, 2) == 5);

    Matrix m3(m2);
    assert(m3.get_height() == 2 && m3.get_width() == 2));
    assert(m3.get(0, 0) == 5);

    m1 = m2;
    assert(m1.get_height() == 2 && m1.get_width() == 2);
    assert(m1.get(0, 0) == 5);

    Matrix m4(std::move(m2));

    assert(m4.get_height() == 2 && m4.get_width() == 2);
    assert(m1.get_height() == 0 && m1.get_width() == 0);

    return 0;
}

int test_matrix2()
{
    Matrix m1(2, 2, 1);
    Matrix m2(2, 2, 2);

    Matrix m3 = m1 + m2;
    assert(m3.get(0, 0) == 3);

    Matrix m4 = m2 - m1;
    assert(m4.get(0, 0) == 1);

    Matrix m5 = m1 * 2;
    assert(m5.get(0, 0) == 2);

    Matrix m6 = 3 * m2;
    assert(m6.get(0, 0) == 6);

    Matrix m7 = m1 / 2;
    assert(m7.get(0, 0) == 0.5);

    m1 += m2;
    assert(m1.get(0, 0) == 3);
    m1 -= m2;
    assert(m1.get(0, 0) == 1);
    m1 *= 2;
    assert(m1.get(0, 0) == 2);
    m1 /= 2.0;
    assert(m1.get(0, 0) == 1);

    return 0;
}

int test_matrix3()
{
    Matrix m1(2, 3, 1);
    Matrix m2(3, 2, 2);

    Matrix m3 = m1 * m2;
    assert(m3.get_height() == 2 && m3.get_width() == 2);
    assert(m3.get(0, 0) == 6);

    Matrix m4 = -m3;
    assert(m4.get(0, 0) == -6);
    return 0;
}

int main()
{
    std::cout << "\nTesting Matrix...\n";

    int result = 0;

    result = test_matrix1();
    if (result)
        return result;

    result = test_matrix2();
    if (result)
        return result;

    result = test_matrix3();
    if (result)
        return result;

    std::cout << "\nAll Matrix tests passed\n";
    return 0;
}