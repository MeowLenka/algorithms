#include <iostream>
#include "rect.hpp"

Rect::Rect()
{
    left = 0;
    right = 0;
    top = 0;
    bottom = 0;
    std::cout << "def kons " << this << '\n';
}

Rect::Rect(int left, int right, int top, int bottom)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    std::cout << "kons w params " << this << '\n';
}

Rect::Rect(const Rect &other)
{
    this->left = other.left;
    this->right = other.right;
    this->top = other.top;
    this->bottom = other.bottom;
    std::cout << "copy kons " << this << '\n';
}

Rect::~Rect()
{
    std::cout << "dest " << this << '\n';
}

int Rect::get_left()
{
    return left;
}

int Rect::get_right()
{
    return right;
}

int Rect::get_top()
{
    return top;
}

int Rect::get_bottom()
{
    return bottom;
}

void Rect::set_all(int left, int right, int top, int bottom)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
}

void Rect::inflate(int amount)
{
    right += amount;
    left -= amount;
    top += amount;
    bottom -= amount;
}

void Rect::inflate(int dw, int dh)
{
    right += dw;
    left -= dw;
    top += dh;
    bottom -= dh;
}

void Rect::inflate(int d_left, int d_right, int d_top, int d_bottom)
{
    right += d_right;
    left -= d_left;
    top += d_top;
    bottom -= d_bottom;
}

void Rect::move(int dx = 0, int dy = 0)
{
    right += dx;
    left += dx;
    top += dy;
    bottom += dy;
}

int Rect::get_width()
{
    return right - left;
}

int Rect::get_height()
{
    return top - bottom;
}

int Rect::get_square()
{
    return get_width() * get_height();
}

void Rect::set_width(int width)
{
    right = left + width;
}

void Rect::set_height(int height)
{
    top = bottom + height;
}

Rect bounding_rect(Rect r1, Rect r2)
{
    int n_left, n_right, n_top, n_bottom;

    if (r1.get_left() < r2.get_left())
        n_left = r1.get_left();
    else
        n_left = r2.get_left();

    if (r1.get_right() > r2.get_right())
        n_right = r1.get_right();
    else
        n_right = r2.get_right();

    if (r1.get_top() > r2.get_top())
        n_top = r1.get_top();
    else
        n_top = r2.get_top();

    if (r1.get_bottom() < r2.get_bottom())
        n_bottom = r1.get_bottom();
    else
        n_bottom = r2.get_bottom();

    return Rect(n_left, n_right, n_top, n_bottom);
}

void print_rect(Rect &r)
{
    std::cout << "left = " << r.get_left() << '\n';
    std::cout << "right = " << r.get_right() << '\n';
    std::cout << "top = " << r.get_top() << '\n';
    std::cout << "bottom = " << r.get_bottom() << '\n';
}