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

