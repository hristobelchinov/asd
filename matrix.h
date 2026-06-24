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

#endif
