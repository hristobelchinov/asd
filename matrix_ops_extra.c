#include "matrix.h"
#include <math.h>

#define EPS 1e-9

static Matrix emptyMatrix(){
    Matrix result = {0};
    return result;
}

int isInvalidMatrix(Matrix m){
    return m.rows == 0 || m.cols == 0;
}

Matrix transposeMatrix(Matrix a){
    Matrix result = {0};

    result.rows = a.cols;
    result.cols = a.rows;

    for(int i = 0; i < a.rows; i++){
        for(int j = 0; j < a.cols; j++){
            result.data[j][i] = a.data[i][j];
        }
    }

    return result;
}

Matrix inverseMatrix(Matrix a){
    Matrix result = {0};

    if(a.rows != a.cols){
        return emptyMatrix();
    }

    if(a.rows > 4){
        return emptyMatrix();
    }

    double det = determinant(a);

    if(fabs(det) < EPS){
        return emptyMatrix();
    }

    int n = a.rows;

    if(n == 1){
        result.rows = 1;
        result.cols = 1;
        result.data[0][0] = 1.0 / a.data[0][0];
        return result;
    }

    Matrix cofactors = {0};
    cofactors.rows = n;
    cofactors.cols = n;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            Matrix minor = getMinor(a, i, j);
            double minorDet = determinant(minor);

            if((i + j) % 2 == 0){
                cofactors.data[i][j] = minorDet;
            } else {
                cofactors.data[i][j] = -minorDet;
            }
        }
    }

    Matrix adjugate = transposeMatrix(cofactors);

    result.rows = n;
    result.cols = n;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            result.data[i][j] = adjugate.data[i][j] / det;
        }
    }

    return result;
}

