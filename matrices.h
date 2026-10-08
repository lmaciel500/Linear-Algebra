
#ifndef CS_107_MATRICES_H
#define CS_107_MATRICES_H

#endif //CS_107_MATRICES_H

static bool AUGMENT = false;
static bool SHOW_STEPS = false;
static bool DO_LU = false;

//helper
void ZeroMatrix(int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
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
int FindLargest(int m, int n, double matrix[m][n]) {
    int largest = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == (int) matrix[i][j]) {
                if (snprintf(NULL, 0, "%d", (int) matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%d", (int) matrix[i][j]) + 1;
                }
            } else {
                if (snprintf(NULL, 0, "%.2lf", matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%.2lf", matrix[i][j]);
                }
            }

        }
    }
    return largest;
}
int DigitsAmount(int a) {
    int digits = 0;
    while (a != 0) {
        a /= 10;
        digits++;
    }
    if (digits == 0) {
        return 1;
    }
    return digits;
}

//misc
int ZeroRowsCount(int m, int n, double matrix[m][n]) {
    int zeroRows = 0;
    for (int i = 0; i < m; i++) {
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
int FindPivot(int column, int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        if (matrix[i][column] == 1) {
            return i;
        }
    }
    for (int i = 0; i < m; i++) {
        if (matrix[i][column] != 0) {
            return i;
        }
    }
    return 0;
}
bool ZeroColumn(int column, int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        if (matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}

//general
void GetInput(int *m, int *n) {
    printf("Enter values for mxn matrix: ");
    scanf(" %d %d", m, n);
    printf("\n");
}
void GetMatrix(int m, int n, double matrix[m][n]) {
    printf("Enter matrix row by row...\n");
    for (int i = 0; i < m; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf(" %lf", &matrix[i][j]);
        }
        //printf("\n");
    }
    printf("\n");
}
void PrintMatrix(int m, int n, double matrix[m][n]) {
    int s = FindLargest(m, n, matrix);
    for (int i = 0; i < m; i++) {
        printf("%c", '|');
        for (int j = 0; j < n; j++) {
            if (AUGMENT && j == n - 1) {
                printf(" %c ", '|');
                //s += 1;
            }
            double x = matrix[i][j];
            if (x == (int) x) {
                printf("%*d", s, (int) x);
            } else {
                printf("%*.2lf", s, x);
            }
        }
        printf(" %c\n", '|');
    }
    printf("\n");
}

//elementary operations
void ScaleRow(int toScale, double c, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d —> (%.2lf)R%d\n", toScale + 1, c, toScale + 1);
        getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toScale][i] = (double) matrix[toScale][i] * c;
    }
    if (SHOW_STEPS) {
        PrintMatrix(m, n, matrix);
    }
}
void ReplaceRow(int toReplace, int replacer, double c, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d —> R%d + (%.1lf)R%d\n", toReplace + 1, toReplace + 1, c, replacer + 1);
        getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toReplace][i] += matrix[replacer][i] * c;
    }
   if (SHOW_STEPS) {
       PrintMatrix(m, n, matrix);
   }
}
void SwapRows(int row1, int row2, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d <—> R%d\n", row1 + 1, row2 + 1);
        getchar();
    }
    double temp[n];

    for (int i = 0; i < n; i++) {
        temp[i] = matrix[row1][i];
    }
    for (int i = 0; i < n; i++) {
        matrix[row1][i] = matrix[row2][i];
    }
    for (int i = 0; i < n; i++) {
        matrix[row2][i] = temp[i];
    }
    if (SHOW_STEPS) {
        PrintMatrix(m, n, matrix);
    }
}
void ScaleMatrix(int c, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("Scale A by %d\n", c);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= c;
        }
    }
    if (SHOW_STEPS) {
        PrintMatrix(m, n, matrix);
    }
}

//algorithm checkers
bool CheckRREF(int m, int n, double matrix[m][n]) {
    int staircase = -1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0 && matrix[i][j] != 1) {
                //printf("Not RREF...\n");
                return false;
            }
            if (matrix[i][j] == 1) {
                if (j <= staircase) {
                    //printf("Not RREF...\n");
                    return false;
                }
                for (int k = 0; k < m; k++) {
                    if (i == k) {
                        continue;
                    }
                    if (matrix[k][j] != 0) {
                        //printf("Not RREF...\n");
                        return false;
                    }
                }
                staircase = j;
                break;
            }
        }
    }
    int zeroRows = ZeroRowsCount(m, n, matrix);
    for (int i = m - 1; i >= m - zeroRows; i--) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                //printf("Not RREF...\n");
                return false;
            }
        }
    }
    printf("Matrix is in RREF.\n\n");
    PrintMatrix(m, n, matrix);
    return true;
}
bool CheckREF(int m, int n, double matrix[m][n]) {
    int staircase = -1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                if (j <= staircase) {
                    //printf("Not REF...\n");
                    return false;
                }
                for (int k = i; k < m; k++) {
                    if (i == k) {
                        continue;
                    }
                    if (matrix[k][j] != 0) {
                        //printf("Not REF...\n");
                        return false;
                    }
                }
                staircase = j;
                break;
            }
        }
    }
    int zeroRows = ZeroRowsCount(m, n, matrix);
    for (int i = m - 1; i >= m - zeroRows; i--) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != 0) {
                //printf("Not REF...\n");
                return false;
            }
        }
    }
    printf("Matrix is in REF.\n\n");
    PrintMatrix(m, n, matrix);
    return true;
}

//checkers
bool CheckIdentity(int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if ((i != j && matrix[i][j] != 0) ||
                (i == j && matrix[i][j] != 1)) {
                printf("Not the identity matrix...\n");
                PrintMatrix(m, n, matrix);
                return false;
                }
        }
    }
    printf("This is the identity matrix...\n");
    PrintMatrix(m, n, matrix);
    return true;
}
bool CheckAugment() {
    char decision;
    printf("Augmented matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        return true;
    } else {
        return false;
    }
}
void CheckSteps() {
    char decision;
    printf("Show steps? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        SHOW_STEPS = true;
    }
}
void CheckLU() {
    char decision;
    printf("LU matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        DO_LU = true;
    }
}

//algorithms
void RREF(int m, int n, double matrix[m][n]) {
    printf("Gaussian Elimination...\n\n");
    PrintMatrix(m, n, matrix);
    int row = 0;
    int column = 0;
    while (!CheckRREF(m, n, matrix)) {
        if (ZeroColumn(column, m, n, matrix)) {
            column++;
            continue;
        }
        if (matrix[row][column] == 0) {
            SwapRows(row, FindPivot(column, m, n, matrix), m, n, matrix);
        }
        if (matrix[row][column] != 1) {
            ScaleRow(row, (double) 1 / matrix[row][column], m, n, matrix);
        }
        for (int i = 0; i < m; i++) {
            if (i == row) {
                continue;
            }
            if (matrix[i][column] != 0) {
                ReplaceRow(i, row, - matrix[i][column] / matrix[row][column], m, n, matrix);
            }
        }
        column++;
        row++;
    }
}
void REF(int m, int n, double matrix[m][n]) {
    bool possibleLU = true;
    printf("Begin REF operations...\n\n");
    PrintMatrix(m, n, matrix);
    int row = 0;
    int column = 0;
    while (!CheckREF(m, n, matrix)) {
        if (!possibleLU) {
            break;
        }
        if (ZeroColumn(column, m, n, matrix)) {
            column++;
            continue;
        }
        for (int i = row; i < m; i++) {
            if (i == row) {
                continue;
            }
            if (DO_LU && matrix[row][column] == 0 && matrix[i][column] != 0) {
                printf("REF for LU impossible...\n");
                possibleLU = false;
                break;
            }
            if (matrix[i][column] != 0) {
                ReplaceRow(i, row, - matrix[i][column] / matrix[row][column], m, n, matrix);
            }
        }
        column++;
        row++;
    }
}

/*
//arithmetic
void MatrixMultiplication(int n, double matrix1[n][n], double matrix2[n][n], double result[n][n]) {
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
void MatrixAddition(int n, double matrix1[n][n], double matrix2[n][n], double result[n][n]) {
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
*/