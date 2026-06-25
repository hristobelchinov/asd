#include <stdio.h>
#include "matrix.h"
#include "encryptedmatrix.h"

static Matrix makeMatrix2x2(double a, double b, double c, double d){
    Matrix m = {0};
    m.rows = 2;
    m.cols = 2;
    m.data[0][0] = a;
    m.data[0][1] = b;
    m.data[1][0] = c;
    m.data[1][1] = d;
    return m;
}

static void title(const char *text){
    printf("\n%s\n", text);
}

int main(){
    Matrix a = makeMatrix2x2(4, 7, 2, 6);
    Matrix b = makeMatrix2x2(1, 2, 3, 4);
    Matrix result;
    Matrix loaded;

    title("Matrix A:");
    printMatrix(a);

    title("Matrix B:");
    printMatrix(b);

    title("Scalar multiplication A * 2:");
    result = scalarMultiply(a, 2);
    printMatrix(result);

    title("Matrix multiplication A * B:");
    result = multiply(a, b);
    printMatrix(result);

    title("Determinant of A:");
    printf("%.2f\n", determinant(a));

    title("Transpose of A:");
    result = transposeMatrix(a);
    printMatrix(result);

    title("Inverse of A:");
    result = inverseMatrix(a);
    if(isInvalidMatrix(result)){
        printf("No inverse.\n");
    } else {
        printMatrix(result);
    }

    title("Matrix division A / B:");
    result = divideMatrices(a, b);
    if(isInvalidMatrix(result)){
        printf("Division is impossible.\n");
    } else {
        printMatrix(result);
    }

    title("Encrypted save/load:");
    if(saveMatrixEncrypted("matrix.enc", &a, "key")){
        printf("Saved.\n");
    } else {
        printf("Save failed.\n");
    }

    if(loadMatrixEncrypted("matrix.enc", &loaded, "key")){
        printf("Loaded matrix:\n");
        printMatrix(loaded);
    } else {
        printf("Load failed.\n");
    }

    return 0;
}

// run command gcc main.c matrix.c matrix_ops_extra.c encryptedmatrix.c -lm -o matrix_program