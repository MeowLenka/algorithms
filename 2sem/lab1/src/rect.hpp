#ifndef RECT_HPP
#define RECT_HPP

class Rect
{
private:
    int left, bottom, width, height;
    // int left, right, top, bottom;

public:
    Rect();
    Rect(int left, int right, int top, int bottom);

    Rect(const Rect &other);

    ~Rect();

    int get_left() const;
    int get_right() const;
    int get_top() const;
    int get_bottom() const;

    int get_width() const;
    int get_height() const;
    int get_square() const;

    void set_all(int left, int right, int top, int bottom);
    void set_width(unsigned int width);
    void set_height(unsigned int height);

    void inflate(int amount);                                       // отодвигает каждую из сторон от центра
    void inflate(int dw, int dh);                                   // отодвигает верхнюю и нижнюю на `dh`, а левую и правую - на `dw`
    void inflate(int d_left, int d_right, int d_top, int d_bottom); // аналогично, но для каждой стороны отдельно.
    void move(int dx=0, int dy=0);
};

Rect bounding_rect(const Rect r1, const Rect r2);
void print_rect(const Rect &r);

#endif