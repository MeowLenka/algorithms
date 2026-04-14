#ifndef MATRIX_HPP
#define MATRIX_HPP

class Matrix
{
private:
    int rows;
    int cols;
    double **data;

public:
    Matrix(int n); // единичная матрица n*n
    Matrix(int m, int n, double fill_value = 0);
    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept; // move-конструктор

    const Matrix &operator=(const Matrix &other);
    Matrix &operator=(Matrix &&other) noexcept; // move-оператор=

    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const double value);
    Matrix &operator/=(const double value);

    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;

    Matrix operator*(const double value) const;
    Matrix operator/(const double value) const;

    ~Matrix();

    double get(int i, int j) const;
    void set(int i, int j, double value);
    int get_height() const;
    int get_width() const;
    void negate();
    void add_in_place(const Matrix &other); // операция `this += other`
    Matrix multiply(const Matrix &other);   // матричное умножение
};

Matrix operator*(double value, const Matrix &mat);
Matrix operator-(const Matrix &mat);

#endif
