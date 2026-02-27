#ifndef RECT_HPP
#define RECT_HPP

class Rect
{
    // верхний левый угол и правый нижний угол
private:
    int left, right, top, bottom;

public:
    Rect();
    Rect(int left, int right, int top, int bottom);

    Rect(const Rect &other);

    ~Rect();

    int get_left();
    int get_right();
    int get_top();
    int get_bottom();
    void set_all(int left, int right, int top, int bottom);

    void inflate(int amount); // отодвигает каждую из сторон от центра
    void inflate(int dw, int dh); // отодвигает верхнюю и нижнюю на `dh`, а левую и правую - на `dw`
    void inflate(int d_left, int d_right, int d_top, int d_bottom); // аналогично, но для каждой стороны отдельно.
    void move(int dx, int dy);

    int get_width();
    int get_height();
    int get_square();

    void set_width(int width);
    void set_height(int height);

    void side_correcting();

};

Rect bounding_rect(Rect r1, Rect r2);
void print_rect(Rect &r);

#endif