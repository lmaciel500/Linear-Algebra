#include <stdbool.h>

static bool AUGMENTED = false;
static bool SHOW_STEPS = false;
static bool DO_LU = false;
static bool LU_DONE = false;
static bool DO_INVERSE = false;

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
double FindLargest(int m, int n, double matrix[m][n]) {
    double scale = 3;
    double largest = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == (int) matrix[i][j]) {
                if (snprintf(NULL, 0, "%d", (int) matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%d", (int) matrix[i][j]) * scale;
                }
            } else {
                if (snprintf(NULL, 0, "%.2lf", matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%.2lf", matrix[i][j]) * scale / 1.6;
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
void DiagonalOnes(int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }
}
void PrepareInverse(int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = n / 2; j < n; j++) {
            if (j == n / 2 + i) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }
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
    //printf("\n");
}
void GetMatrix(int m, int n, double matrix[m][n]) {
    printf("Enter matrix row by row...\n");
    printf("(%d rows, %d columns)\n\n", m, n);
    for (int i = 0; i < m; i++) {
        printf("%d: ", i + 1);
        if (DO_INVERSE) {
            for (int j = 0; j < n / 2; j++) {
                scanf(" %lf", &matrix[i][j]);
            }
        } else {
            for (int j = 0; j < n; j++) {
                scanf(" %lf", &matrix[i][j]);
            }
        }
        //printf("\n");
    }
}
void PrintMatrix(int m, int n, double matrix1[m][n], double matrix2[m][n]) {
    int a, b, s;
    a = FindLargest(m, n, matrix1);
    if (LU_DONE) {
        b = FindLargest(m, n, matrix2);
    } else {
        b = 0;
    }
    if (a > b) {
        s = a;
    } else {
        s = b;
    }
    for (int i = 0; i < m; i++) {
        printf("%c", '|');
        for (int j = 0; j < n; j++) {
            if (AUGMENTED && j == n - 1) {
                printf(" %c ", '|');
                //s += 1;
            }
            if (DO_INVERSE && j == n / 2) {
                printf(" %c ", '|');
            }
            double x = matrix1[i][j];
            if (x == (int) x) {
                printf("%*d", s, (int) x);
            } else {
                printf("%*.2lf", s, x);
            }
        }
        if (DO_LU && LU_DONE) {
            //printf("%*c", s, ' ');
            printf(" %c", '|');
            printf(" %c", '|');
            //printf("%*c", s, ' ');
            for (int j = 0; j < n; j++) {
                double x = matrix2[i][j];
                if (x == (int) x) {
                    printf("%*d", s, (int) x);
                } else {
                    printf("%*.2lf", s, x);
                }
            }
        }
        if (i + 1 == n && DO_LU && LU_DONE) {
            printf(" %c", '|');
            printf(" = LU\n");
        } else {
            printf(" %c\n", '|');
        }
    }
    printf("\n");
}
void PrintLU(int n, double matrixL[n][n], double matrixU[n][n]) {
    int s;
    int a = FindLargest(n, n, matrixL);
    int b = FindLargest(n, n, matrixL);
    if (a > b) {
        s = a;
    } else {
        s = b;
    }
    for (int i = 0; i < n; i++) {
        printf("%c", '|');
        for (int j = 0; j < n; j++) {
            double x = matrixL[i][j];
            if (x == (int) x) {
                printf("%*d", s, (int) x);
            } else {
                printf("%*.2lf", s, x);
            }
        }
        //printf("%*c", s, ' ');
        printf(" %c", '|');
        printf(" %c", '|');
        //printf("%*c", s, ' ');
        for (int j = 0; j < n; j++) {
            double x = matrixU[i][j];
            if (x == (int) x) {
                printf("%*d", s, (int) x);
            } else {
                printf("%*.2lf", s, x);
            }
        }
        if (i + 1 == n) {
            printf(" %c", '|');
            printf(" = LU\n");
        } else {
            printf(" %c\n", '|');
        }
    }
    printf("\n");
}

//elementary operations
void ScaleRow(int toScale, double c, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d —> (%.2lf)R%d\n", toScale + 1, c, toScale + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toScale][i] = (double) matrix[toScale][i] * c;
    }
    if (SHOW_STEPS) {
        PrintMatrix(m, n, matrix, NULL);
    }
}
void ReplaceRow(int toReplace, int replacer, double c, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d —> R%d + (%.1lf)R%d\n", toReplace + 1, toReplace + 1, c, replacer + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toReplace][i] += matrix[replacer][i] * c;
    }
    if (SHOW_STEPS) {
        PrintMatrix(m, n, matrix, NULL);
    }
}
void SwapRows(int row1, int row2, int m, int n, double matrix[m][n]) {
    if (SHOW_STEPS) {
        printf("R%d <—> R%d\n", row1 + 1, row2 + 1);
        //getchar();
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
        PrintMatrix(m, n, matrix, NULL);
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
        PrintMatrix(m, n, matrix, NULL);
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
    if (DO_LU) {
        LU_DONE = true;
    }
    return true;
}

//checkers
bool CheckIdentity(int m, int n, double matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if ((i != j && matrix[i][j] != 0) ||
                (i == j && matrix[i][j] != 1)) {
                printf("Not the identity matrix...\n");
                PrintMatrix(m, n, matrix, NULL);
                return false;
                }
        }
    }
    printf("This is the identity matrix...\n");
    PrintMatrix(m, n, matrix, NULL);
    return true;
}
bool CheckAugment() {
    char decision;
    printf("Augmented matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        AUGMENTED = true;
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
bool CheckLU() {
    char decision;
    printf("LU matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        DO_LU = true;
        return true;
    }
    return false;
}
bool CheckInverse() {
    char decision;
    printf("Inverse matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        DO_INVERSE = true;
        return true;
    }
    return false;
}

//algorithms
void RREF(int m, int n, double matrix[m][n]) {
    if (DO_INVERSE) {
        PrepareInverse(m, n, matrix);
    }
    printf("Gaussian Elimination...\n");
    PrintMatrix(m, n, matrix, NULL);
    int row = 0;
    int column = 0;
    while (!CheckRREF(m, n, matrix)) {
        if (ZeroColumn(column, m, n, matrix)) {
            column++;
            continue;
        }
        if (matrix[row][column] == 0) {
            int newRow = FindPivot(column, m, n, matrix);
            SwapRows(row, newRow, m, n, matrix);
        }
        if (matrix[row][column] != 1) {
            double c = 1 / matrix[row][column];
            ScaleRow(row, c, m, n, matrix);
        }
        for (int i = 0; i < m; i++) {
            if (i == row) {
                continue;
            }
            if (matrix[i][column] != 0) {
                double c = - matrix[i][column] / matrix[row][column];
                ReplaceRow(i, row, c, m, n, matrix);
            }
        }
        column++;
        row++;
    }
    PrintMatrix(m, n, matrix, NULL);
}
void REF(int m, int n, double matrixL[m][n], double matrixU[m][n]) {
    bool possibleLU = true;
    printf("Begin REF operations...\n");
    PrintMatrix(m, n, matrixU, NULL);
    int row = 0;
    int column = 0;
    while (!CheckREF(m, n, matrixU)) {
        if (!possibleLU) {
            break;
        }
        if (ZeroColumn(column, m, n, matrixU)) {
            column++;
            continue;
        }
        for (int i = row; i < m; i++) {
            if (i == row) {
                continue;
            }
            if (DO_LU && matrixU[row][column] == 0 && matrixU[i][column] != 0) {
                printf("REF for LU impossible...\n");
                possibleLU = false;
                break;
            }
            if (matrixU[i][column] != 0) {
                double c = - matrixU[i][column] / matrixU[row][column];
                if (DO_LU) {
                    matrixL[i][row] = - c;
                }
                ReplaceRow(i, row, c, m, n, matrixU);
            }
        }
        column++;
        row++;
    }
    PrintMatrix(m, n, matrixL, matrixU);
}

bool Start() {
    int m, n;
    GetInput(&m, &n);
    if (m < 1 || n < 1) {
        return false;
    }
    CheckSteps();
    if (CheckAugment()) {
        n++;
        double matrix[m][n];
        GetMatrix(m, n, matrix);
        RREF(m, n, matrix);
    } else if (CheckInverse()) {
        if (m != n) {
            DO_INVERSE = false;
        } else {
            n *= 2;
            double matrix[m][n];
            GetMatrix(m, n, matrix);
            RREF(m, n, matrix);
        }
    } else if (CheckLU()) {
        if (m != n) {
            DO_LU = false;
        } else {
            double matrixL[m][n];
            double matrixU[m][n];
            DiagonalOnes(m, n, matrixL);
            GetMatrix(m, n, matrixU);
            REF(m, n, matrixL, matrixU);
        }
    } else {
        double matrix[m][n];
        GetMatrix(m, n, matrix);
        RREF(m, n, matrix);
    }
    AUGMENTED = false;
    SHOW_STEPS = false;
    DO_LU = false;
    LU_DONE = false;
    DO_INVERSE = false;
    return true;
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