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

Matrix::~Matrix()
{
    for (int i = 0; i < rows; i++)
        delete[] data[i];
    delete[] data;
}

double Matrix::get(int i, int j)
{
    return data[i][j];
}

void Matrix::set(int i, int j, double value)
{
    data[i][j] = value;
}
int Matrix::get_height()
{
    return rows;
}

int Matrix::get_width()
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

void Matrix::add_in_place(Matrix &other)
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

Matrix Matrix::multiply(Matrix &other)
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