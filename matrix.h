#ifndef MATRIX_H
#define MATRIX_H

#define MAX 10

typedef struct {
    int rows;
    int cols;
    double data[MAX][MAX];
} Matrix;

void readMatrix(Matrix *m);
void printMatrix(Matrix m);

Matrix scalarMultiply(Matrix m, double scalar);
Matrix multiply(Matrix a, Matrix b);
Matrix getMinor(Matrix m, int skipRow, int skipCol);
double determinant(Matrix m);

Matrix transposeMatrix(Matrix a);
Matrix inverseMatrix(Matrix a);
Matrix divideMatrices(Matrix a, Matrix b);
int isInvalidMatrix(Matrix m);

#endif