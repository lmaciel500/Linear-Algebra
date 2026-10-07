#include <stdio.h>

void PrintMatrix(int n, int matrix[n][n]);
void MatrixAddition(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]);
void MatrixMultiplication(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]);
int GetInput();
void GetMatrix(int n, int matrix[n][n]);
void ZeroMatrix(int n, int matrix[n][n]);
void SwapRows(int n, int one, int two, int matrix1[n][n]);

int main(void) {
    printf("\n");
    int n = GetInput();

    int result[n][n];
    ZeroMatrix(n, result);

    int matrix1[n][n];
    int matrix2[n][n];

    GetMatrix(n, matrix1);
    PrintMatrix(n, matrix1);

    GetMatrix(n, matrix2);
    PrintMatrix(n, matrix2);

    //MatrixAddition(n, matrix1, matrix2, result);
    MatrixMultiplication(n, matrix1, matrix2, result);

    return 0;
}
void SwapRows(int one, int two, int n, int matrix[n][n]) {
    int row1 = one;
    int row2 = two;

    int temp[n];

    for (int i = 0; i < n; i++) {
        temp[i] = matrix[row1][i];
    }
    for (int i = 0; i < n; i++) {
        matrix[row1][i] = matrix[row2][i];
    }
    for (int i = 0; i < n; i++) {
        matrix[row2][i] = temp[i];
    }
}
void MatrixMultiplication(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]) {
    PrintMatrix(n, matrix1);
    printf("X\n");
    PrintMatrix(n, matrix2);
    printf("=\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    PrintMatrix(n, result);
    printf("\n");
}
void MatrixAddition(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]) {
    PrintMatrix(n, matrix1);
    printf("+\n");
    PrintMatrix(n, matrix2);
    printf("=\n");

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
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}
void ZeroMatrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}