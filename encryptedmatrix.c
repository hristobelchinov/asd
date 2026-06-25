#include "encryptedmatrix.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_SIGNATURE "MATRIX_"

static int isMatrixValid(const Matrix *matrix){
    return matrix != NULL && matrix->rows>=1 && matrix->rows<=MAX && matrix->cols>=1 && matrix->cols <=MAX;
}

static int writeEncrypted(FILE *file, const char *text, const char *key, size_t *keyPosition){
    size_t keyLength = strlen(key);

    for(size_t i=0; text[i]!='\0'; i++){
        unsigned char plainByte = (unsigned char) text[i];
        unsigned char keyByte = (unsigned char) key[*keyPosition % keyLength];
        unsigned char encryptedByte = plainByte^keyByte;

        if(fputc(encryptedByte, file)==EOF){
            return 0;
        }

        (*keyPosition)++;
    }

    return 1;
}

static void decryptBuffer(unsigned char *buffer, size_t size, const char *key){
    size_t keyLength = strlen(key);

    for(size_t i=0; i<size; i++){
        buffer[i]^=(unsigned char) key[i%keyLength];
    }
}


int saveMatrixEncrypted(const char *filePath, const Matrix *matrix, const char *key){
    if(filePath == NULL || key == NULL || key[0] == '\0' || !isMatrixValid(matrix)){
        return 0;
    }

    FILE *file = fopen(filePath, "wb");
    if(file==NULL){
        return 0;
    }

    size_t keyPosition = 0;
    char line[512];

    if(!writeEncrypted(file, FILE_SIGNATURE "\n", key, &keyPosition)){
        fclose(file);
        return 0;
    }

    int written = snprintf(line, sizeof(line), "%d %d\n", matrix->rows, matrix->cols);
    if(written<0 || (size_t) written >= sizeof(line) || !writeEncrypted(file, line, key, &keyPosition)){
        fclose(file);
        return 0;
    }

    for(int i=0; i<matrix->rows; i++){
        size_t used = 0;
        line[0] = '\0';

        for(int j=0; j<matrix->cols; j++){
            written = snprintf(
                line+used,
                sizeof(line)-used,
                "%g ",
                matrix->data[i][j]
            );

            if(written<0 || (size_t)written>=sizeof(line)-used){
                fclose(file);
                return 0;
            }

            used+=(size_t)written;
        }

        if(used+1>=sizeof(line)){
            fclose(file);
            return 0;
        }

        line[used++] = '\n';
        line[used] = '\0';

        if(!writeEncrypted(file, line, key, &keyPosition)){
            fclose(file);
            return 0;
        }
    }

    if(fclose(file) != 0){
        return 0;
    }

    return 1;
}

int loadMatrixEncrypted(const char *filePath, Matrix *matrix, const char *key){
    if(filePath == NULL || matrix == NULL || key == NULL || key[0] == '\0') {
        return 0;
    }

    FILE *file = fopen(filePath, "rb");
    if(file==NULL){
        return 0;
    }

    if(fseek(file, 0, SEEK_END) !=0){
        fclose(file);
        return 0;
    }

    long fileSizeLong = ftell(file);
    if(fileSizeLong<=0){
        fclose(file);
        return 0;
    }

    if(fseek(file, 0, SEEK_SET)!=0){
        fclose(file);
        return 0;
    }

    size_t fileSize = (size_t) fileSizeLong;
    unsigned char *buffer = malloc(fileSize+1);
    if(buffer == NULL){
        fclose(file);
        return 0;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    fclose(file);

    if(bytesRead != fileSize){
        free(buffer);
        return 0;
    }

    decryptBuffer(buffer, fileSize, key);
    buffer[fileSize] = '\0';

    char *text = (char *) buffer;
    size_t signatureLength = strlen(FILE_SIGNATURE);

    if(strncmp(text, FILE_SIGNATURE, signatureLength) != 0){
        free(buffer);
        return 0;
    }

    text += signatureLength;

    char *end = NULL;
    long rows = strtol(text, &end, 10);
    if(end == text){
        free(buffer);
        return 0;
    }

    text = end;

    long cols = strtol(text, &end, 10);
    if(end==text || rows<1 || rows>MAX || cols<1 || cols>MAX){
        free(buffer);
        return 0;
    }

    Matrix loaded = {0};
    loaded.rows = (int) rows;
    loaded.cols = (int) cols;
    text = end;

    for(int i=0; i<loaded.rows; i++){
        for(int j=0; j<loaded.cols; j++){

            double value = strtod(text, &end);
            if(end==text || !isfinite(value)){
                free(buffer);
                return 0;
            }

            loaded.data[i][j] = value;
            text = end;
        }
    }

   while(*text != '\0' && isspace((unsigned char)*text)){
    text++;
    }

    if(*text != '\0'){
        free(buffer);
        return 0;
    }

    *matrix=loaded;
    free(buffer);
    return 1;
}
