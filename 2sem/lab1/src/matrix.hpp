#ifndef MATRIX_HPP
#define MATRIX_HPP

class Matrix
{
public:
    int rows;
    int cols;
    double **data;

public:
    Matrix(int n); // единичная матрица n*n
    Matrix(int m, int n, double fill_value = 0);

    Matrix(const Matrix &other); 
    ~Matrix();

    double get(int i, int j);
    void set(int i, int j, double value);
    int get_height();
    int get_width();
    void negate();
    void add_in_place(Matrix &other); // операция `this += other`
    Matrix multiply(Matrix &other);   // матричное умножение
    
};

#endif
