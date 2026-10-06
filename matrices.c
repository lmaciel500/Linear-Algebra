#include <stdio.h>

void PrintMatrix(int n, int matrix[n][n]);
void MatrixMultiplication(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]);
int GetInputN();
void GetMatrix(int n, int matrix[n][n]);

int main(void) {
    printf("\n");
    int n = GetInputN();

    int matrix1[n][n];
    int matrix2[n][n];
    int result[n][n];

    GetMatrix(n, matrix1);
    PrintMatrix(n, matrix1);

    GetMatrix(n, matrix2);
    PrintMatrix(n, matrix2);

    MatrixMultiplication(n, matrix1, matrix2, result);

    return 0;
}
void MatrixMultiplication(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]) {
    PrintMatrix(n, matrix1);
    printf("\nX\n");
    PrintMatrix(n, matrix2);
    printf("\n=\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    PrintMatrix(n, result);
    printf("\n");
}
int GetInputN() {
    int n;
    printf("Enter n for nxn square matrix: ");
    scanf(" %d", &n);
    printf("\n");
    return n;
}
void GetMatrix(int n, int matrix[n][n]) {
    printf("Enter matrix row by row...\n");
    for (int i = 0; i < n; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf(" %d", &matrix[i][j]);
        }
        //printf("\n");
    }
    printf("\n");
}
void PrintMatrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}