#include <stdio.h>

#define MAX 10  

typedef struct{
    int rows;
    int cols;
    double data[MAX][MAX];
}Matrix;

void readMatrix(Matrix *m){
    printf("Въведи брой редове и колони: ");
    scanf("%d %d", &m->rows, &m->cols);
    printf("Въведи елементите ред по ред:\n");
    for(int i=0; i<m->rows; i++)
        for(int j=0; j<m->cols; j++)
            scanf("%lf", &m->data[i][j]);
}

void printMatrix(Matrix m){
    for(int i=0; i<m.rows; i++){
        for(int j=0; j<m.cols; j++)
            printf("%.2f ", m.data[i][j]);
        printf("\n");
    }
}

Matrix scalarMultiply(Matrix m, double scalar){
    Matrix result=m;
    for(int i=0; i<m.rows; i++)
        for(int j=0; j<m.cols; j++)
            result.data[i][j]=m.data[i][j]*scalar;
    return result;
}

Matrix multiply(Matrix a, Matrix b){
    Matrix result;
    result.rows=a.rows;
    result.cols=b.cols;
    for(int i=0; i<a.rows; i++)
        for(int j=0; j<b.cols; j++){
            result.data[i][j]=0;
            for(int k=0; k<a.cols; k++)
                result.data[i][j]+=a.data[i][k]*b.data[k][j];
        }
    return result;
}

Matrix getMinor(Matrix m, int skipRow, int skipCol){
    Matrix minor;
    minor.rows=m.rows-1;
    minor.cols=m.cols-1;
    int r=0;
    for(int i=0; i<m.rows; i++){
        if(i==skipRow) continue;
        int c=0;
        for(int j=0; j<m.cols; j++){
            if(j==skipCol) continue;
            minor.data[r][c]=m.data[i][j];
            c++;
        }
        r++;
    }
    return minor;
}

double determinant(Matrix m){
    int n=m.rows;
    if(n==1) return m.data[0][0];
    if(n==2) return m.data[0][0]*m.data[1][1]-m.data[0][1]*m.data[1][0];
    double det=0;
    int sign=1;
    for(int j=0; j<n; j++){
        Matrix minor=getMinor(m, 0, j);
        det+=sign*m.data[0][j]*determinant(minor);
        sign=-sign;
    }
    return det;
}

int main(){

    return 0;
}
