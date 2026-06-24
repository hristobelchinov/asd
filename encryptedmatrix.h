#ifndef ENCRYPTED_MATRIX_H
#define ENCRYPTED_MATRIX_H
#include "matrix.h"

int saveMatrixEncrypted(const char *filePath, const Matrix *matrix, const char *key);

int loadMatrixEncrypted(const char *filePath, Matrix *matrix, const char *key);

#endif
