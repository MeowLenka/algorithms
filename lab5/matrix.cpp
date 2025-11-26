#include "matrix.h"
#include "iostream"

void m_zero(Matrix *A, int m, int n)
{
    A->rows = m;
    A->cols = n;
    A->data = new double *[m];
    for (int i = 0; i < m; i++)
    {
        A->data[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            A->data[i][j] = 0;
        }
    }
}

void m_free(Matrix *A)
{
    for (int i = 0; i < A->rows; i++)
        delete[] A->data[i];
    delete [] A->data;
}

void m_copy(const Matrix *src, Matrix *dst)
{
    int ms = src->rows, ns = src->cols;
    m_zero(dst, ms, ns);
    for (int i = 0; i < ms; i++)
    {
        for (int j = 0; j < ns; j++)
        {
            dst->data[i][j] = src->data[i][j];
        }
    }
}

int m_width(const Matrix *A)
{
    return A->cols;
}

int m_height(const Matrix *A)
{
    return A->rows;
}

double m_get(const Matrix *A, int i, int j)
{
    return A->data[i][j];
}

void m_set(Matrix *A, int i, int j, double value)
{
    A->data[i][j] = value;
}

int m_add(Matrix *A, const Matrix *B)
{
    if (m_width(A) == m_width(B) && m_height(A) == m_height(B))
    {
        for (int i = 0; i < A->rows; i++)
        {
            for (int j = 0; A->cols; j++)
            {
                A->data[i][j] += B->data[i][j];
            }
        }
        return 0;
    }
    return 1;
}

void m_neg(Matrix *A)
{
    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; A->cols; j++)
        {
            A->data[i][j] *= -1;
        }
    }
}
int m_mult(const Matrix *A, const Matrix *B, Matrix *dst)
{
    if (m_width(A) == m_height(B))
    {
        for (int i = 0; i < A->rows; i++)
        { // строки A
            for (int j = 0; j < B->cols; j++)
            { // столбцы B
                double sum = 0;
                for (int k = 0; k < A->cols; k++)
                {
                    sum += A->data[i][k] * B->data[k][j];
                }
                dst->data[i][j] = sum;
            }
        }
        return 0;
    }
    return 1;
}

void m_print(const Matrix &A){
    for(int i = 0; i < A.rows; i++){
        for (int j = 0; j < A.cols; j++){
            printf("%f ", A.data[i][j]);
        }
        printf("\n");
    }
}