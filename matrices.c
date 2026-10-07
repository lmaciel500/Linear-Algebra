#include <stdio.h>
#include <stdbool.h>

void ZeroArray(int n, int array[n]);
int ZeroRowsCount(int n, int matrix[n][n]);
void PrintMatrix(int n, int matrix[n][n]);
void MatrixAddition(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]);
void MatrixMultiplication(int n, int matrix1[n][n], int matrix2[n][n], int result[n][n]);
int GetInput();
void GetMatrix(int n, int matrix[n][n]);
void ZeroMatrix(int n, int matrix[n][n]);
void SwapRows(int row1, int row2, int n, int matrix1[n][n]);
void ReplaceRow(int toReplace, int replacer, int c, int n, int matrix[n][n]);
void ScaleRow(int toScale, double c, int n, int matrix[n][n]);
void RREF(int n, int matrix[n][n]);
int FindPivot(int column, int n, int matrix[n][n]);
void ScaleMatrix(int c, int n, int matrix[n][n]);
bool CheckIdentity(int n, int matrix[n][n]);
bool CheckRREF(int n, int matrix[n][n]);

int main(void) {
    printf("\n");
    int n = GetInput();

    int result[n][n];
    ZeroMatrix(n, result);

    int matrix1[n][n];
    GetMatrix(n, matrix1);

    //ScaleMatrix(2, n, matrix1);
    //CheckIdentity(n, matrix1);

    RREF(n, matrix1);

    return 0;
}

void RREF(int n, int matrix[n][n]) {
    printf("Gaussian Elimination...\n");
    PrintMatrix(n, matrix);
    int row = 0;
    int column = 0;
    while (!CheckRREF(n, matrix)) {
        ScaleRow(row, (double) 1 / matrix[row][column], n, matrix);
        for (int i = 0; i < n; i++) {
            if (i == row) {
                continue;
            } else {
                ReplaceRow(i, row, - matrix[i][column] / matrix[row][column], n, matrix);
            }
        }
        column++;
        row++;
    }
}

//elementary operations
void ScaleRow(int toScale, double c, int n, int matrix[n][n]) {
    printf("R%d —> (%.2lf)R%d\n", toScale + 1, c, toScale + 1);
    getchar();
    for (int i = 0; i < n; i++) {
        matrix[toScale][i] = (double) matrix[toScale][i] * c;
    }
    PrintMatrix(n, matrix);
}
void ReplaceRow(int toReplace, int replacer, int c, int n, int matrix[n][n]) {
    printf("R%d —> R%d + (%d)R%d\n", toReplace + 1, toReplace + 1, c, replacer + 1);
    getchar();
    for (int i = 0; i < n; i++) {
        matrix[toReplace][i] += matrix[replacer][i] * c;
    }
    PrintMatrix(n, matrix);
}
void SwapRows(int row1, int row2, int n, int matrix[n][n]) {
    printf("R%d <—> R%d\n", row1 + 1, row2 + 1);
    getchar();
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
    PrintMatrix(n, matrix);
}

//misc
bool CheckRREF(int n, int matrix[n][n]) {
    int staircase = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                printf("Not RREF...\n");
                return false;
            }
            if (matrix[i][j] == 1) {
                if (j <= staircase) {
                    printf("Not RREF...\n");
                    return false;
                }
                for (int k = 0; k < n; k++) {
                    if (i == k) {
                        continue;
                    }
                    if (matrix[k][j] != 0) {
                        printf("Not RREF...\n");
                        return false;
                    }
                }
                staircase = j;
                break;
            }
        }
    }
    int zeroRows = ZeroRowsCount(n, matrix);
    for (int i = n - 1; i >= n - zeroRows; i--) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                printf("Not RREF...\n");
                return false;
            }
        }
    }
    printf("Matrix is in RREF.\n");
    PrintMatrix(n, matrix);
    return true;
}
int ZeroRowsCount(int n, int matrix[n][n]) {
    int zeroRows = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                break;
            }
            if (j == n - 1) {
                zeroRows++;
            }
        }
    }
    return zeroRows;
}
bool CheckIdentity(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i != j && matrix[i][j] != 0) ||
                (i == j && matrix[i][j] != 1)) {
                printf("Not the identity matrix...\n");
                PrintMatrix(n, matrix);
                return false;
            }
        }
    }
    printf("This is the identity matrix...\n");
    PrintMatrix(n, matrix);
    return true;
}
int FindPivot(int column, int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        if (matrix[i][column] == 1) {
            return i;
        }
    }
    for (int i = 0; i < n; i++) {
        if (matrix[i][column] != 0) {
            return i;
        }
    }
    return 0;
}
void ScaleMatrix(int c, int n, int matrix[n][n]) {
    printf("Scale A by %d\n", c);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= c;
        }
    }
}

//initials
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
int GetInput() {
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
        printf("%*c", n, '|');
        for (int j = 0; j < n; j++) {
            printf("%*d", n,  matrix[i][j]);
        }
        printf("%*c\n", n, '|');
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
void ZeroArray(int n, int array[n]) {
    for (int i = 0; i < n; i++) {
        array[i] = 0;
    }
}