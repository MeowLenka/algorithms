#include <iostream>

class A {
public:
    A(int &);
};

// Отметьте корректные варианты создания объекта
A* pa = new A();

int n = 1; A a(n); <-

int n = 1; A a = n; <-

A a;

int n = 1; A* pa = new A(n); <-

A a(1);