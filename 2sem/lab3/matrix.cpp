#include <iostream>
#include "matrix.hpp"

Matrix::Matrix(int n)
{
    rows = n;
    cols = n;
    data = new double *[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = (i == j) ? 1 : 0;
        }
    }
}

Matrix::Matrix(int m, int n, double fill_value)
{
    rows = m;
    cols = n;
    data = new double *[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = fill_value;
        }
    }
}

Matrix::Matrix(const Matrix &other)
{
    int o_rows = other.rows;
    int o_cols = other.cols;
    data = new double *[o_rows];
    for (int i = 0; i < o_rows; i++)
    {
        data[i] = new double[o_cols];
        for (int j = 0; j < o_cols; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix::Matrix(Matrix &&other) noexcept : rows(other.rows), cols(other.cols), data(other.data)
{
    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows; i++)
        delete[] data[i];
    delete[] data;
}

const Matrix &Matrix::operator=(const Matrix &other)
{
    if (this != &other)
    {
        delete[] data;
        rows = other.rows;
        cols = other.cols;
        data = new double *[rows];
        for (int i = 0; i < rows; i++)
        {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++)
            {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept
{
    if (this != &other)
    {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;

        rows = other.rows;
        cols = other.cols;
        data = other.data;

        other.rows = 0;
        other.cols = 0;
        other.data = nullptr;
    }
    return *this;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (other.cols != cols || other.rows != rows)
    {
        throw "Matrix size error (+=)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < rows; j++)
                data[i][j] += other.data[i][j];
    }
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    if (other.cols != cols || other.rows != rows)
    {
        throw "Matrix size error (-=)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < rows; j++)
                data[i][j] -= other.data[i][j];
    }
    return *this;
}

Matrix &Matrix::operator*=(const double value)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < rows; j++)
            data[i][j] *= value;
    return *this;
}

Matrix &Matrix::operator/=(const double value)
{
    if (value == 0)
    {
        throw "Divide by zero error (/=)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < rows; j++)
                data[i][j] /= value;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const
{
    Matrix dst(rows, cols, 0);
    if (other.cols != cols || other.rows != rows)
    {
        throw "Matrix size error (+)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                dst.data[i][j] = other.data[i][j] + data[i][j];
            }
        }
    }
    return dst;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    Matrix dst(rows, cols, 0);

    if (other.cols != cols || other.rows != rows)
    {
        throw "Matrix size error (-)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                dst.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
    }
    return dst;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    Matrix dst(rows, other.cols, 0);
    if (cols != other.rows)
    {
        throw "Matrix size error (*)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
        { // строки A
            for (int j = 0; j < other.cols; j++)
            { // столбцы B
                double sum = 0;
                for (int k = 0; k < cols; k++)
                    sum += data[i][k] * other.get(k, j);

                dst.set(i, j, sum);
            }
        }
    }
    return dst;
}

Matrix Matrix::operator*(const double value) const
{
    Matrix dst(rows, cols, 0);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            dst.data[i][j] = data[i][j] * value;
    return dst;
}

Matrix Matrix::operator/(const double value) const
{
    Matrix dst(rows, cols, 0);
    if (value == 0)
    {
        throw "Divide by zero error (/)";
    }
    else
    {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                dst.data[i][j] = data[i][j] / value;
    }
    return dst;
}

double Matrix::get(int i, int j) const
{
    return data[i][j];
}

void Matrix::set(int i, int j, double value)
{
    data[i][j] = value;
}
int Matrix::get_height() const
{
    return rows;
}

int Matrix::get_width() const
{
    return cols;
}
void Matrix::negate()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            data[i][j] *= -1;
        }
    }
}

void Matrix::add_in_place(const Matrix &other)
{
    if (rows == other.rows && cols == other.cols)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
                data[i][j] += other.get(i, j);
        }
    }
}

Matrix Matrix::multiply(const Matrix &other)
{
    Matrix dst(rows, other.cols, 0);

    if (cols == other.rows)
    {
        for (int i = 0; i < rows; i++)
        { // строки A
            for (int j = 0; j < other.cols; j++)
            { // столбцы B
                double sum = 0;
                for (int k = 0; k < cols; k++)
                    sum += data[i][k] * other.get(k, j);

                dst.set(i, j, sum);
            }
        }
    }
    return dst;
}

Matrix operator*(double value, const Matrix &mat) { return mat * value; }

Matrix operator-(const Matrix &mat)
{
    Matrix result(mat);
    result.negate();
    return result;
}