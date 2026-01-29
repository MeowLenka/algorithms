struct Matrix
{
    int rows;
    int cols;
    double **data; 
};


void m_zero(Matrix *A, int m, int n);

void m_free(Matrix *A);

void m_copy(const Matrix *src, Matrix *dst);

int m_width(const Matrix *A);

int m_height(const Matrix *A);

double m_get(const Matrix* A, int i, int j);

void m_set(Matrix *A, int i, int j, double value);


int m_add(Matrix *A, const Matrix *B);

void m_neg(Matrix *A);

int m_mult(const Matrix *A, const Matrix *B, Matrix *dst);

void m_print(const Matrix& A);