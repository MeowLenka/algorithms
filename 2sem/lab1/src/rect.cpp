#include <iostream>
#include "rect.hpp"


Rect::Rect() : Rect(0, 0, 0, 0)
{
    // std::cout << "def kons " << this << '\n';
}

Rect::Rect(int left, int right, int top, int bottom) : left(left), width(right - left), height(top - bottom), bottom(bottom)
{
    if (left > right || bottom > top)
        throw "wrong rect";
    // std::cout << "kons w params " << this << '\n';
}

Rect::Rect(const Rect &other) : Rect(other.get_left(), other.get_right(), other.get_top(), other.get_bottom()) {
                                    // std::cout << "copy kons " << this << '\n';
                                };

Rect::~Rect()
{
    // std::cout << "dest " << this << '\n';
}

int Rect::get_left() const { return left; }

int Rect::get_right() const
{
    // return right;
    return left + width;
}

int Rect::get_top() const
{
    // return top;
    return bottom + height;
}

int Rect::get_bottom() const { return bottom; }

int Rect::get_width() const
{
    // return right - left;
    return width;
}

int Rect::get_height() const
{
    // return top - bottom;
    return height;
}

int Rect::get_square() const { return get_width() * get_height(); }

void Rect::set_all(int left, int right, int top, int bottom)
{
    if (left > right || bottom > top)
        throw "wrong rect";
    // this->left = left;
    // this->right = right;
    // this->top = top;
    // this->bottom = bottom;
    this->left = left;
    this->width = right - left;
    this->height = top - bottom;
    this->bottom = bottom;
}

void Rect::set_width(unsigned int width)
{
    // right = left + width;
    this->width = width;
}

void Rect::set_height(unsigned int height)
{
    // top = bottom + height;
    this->height = height;
}

void Rect::inflate(int amount)
{
    inflate(amount, amount, amount, amount);
}

void Rect::inflate(int dw, int dh)
{
    inflate(dw, dw, dh, dh);
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

void Rect::side_correcting()
{
    int temp;
    if (left > right)
    {
        temp = left;
        left = right;
        right = left;      
    } 
    if (bottom > top)
    {
        temp = bottom;
        bottom = top;
        top = temp;

    }
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

void print_rect(const Rect &r)
{
    std::cout << "left = " << r.get_left() << '\n';
    std::cout << "right = " << r.get_right() << '\n';
    std::cout << "top = " << r.get_top() << '\n';
    std::cout << "bottom = " << r.get_bottom() << '\n';
}