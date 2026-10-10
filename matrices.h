#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int rows;
    int columns;
    double matrix[100][100];
} Matrix;

typedef struct {
    int amount;
    Matrix matrices[10];
} Collection;

static Collection GLOBAL;

static bool AUGMENTED = false;

static bool SHOW_STEPS = false;

static bool DO_LU = false;
static bool LU_DONE = false;

static bool DO_INVERSE = false;

static bool DO_MULT = false;
static bool MULT_DONE = false;

//helper
void ZeroMatrix(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}
int FindLargestEntry(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    double largest = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == (int) matrix[i][j]) {
                if (snprintf(NULL, 0, "%d", (int) matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%d", (int) matrix[i][j]);
                }
            } else {
                if (snprintf(NULL, 0, "%.2lf", matrix[i][j]) > largest) {
                    largest = snprintf(NULL, 0, "%.2lf", matrix[i][j]);
                }
            }

        }
    }
    return (int) largest;
}
void DiagonalOnes(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
void PrepareInverse(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
void ResetFlags() {
    AUGMENTED = false;
    SHOW_STEPS = false;
    DO_LU = false;
    LU_DONE = false;
    DO_INVERSE = false;
    DO_MULT = false;
    MULT_DONE = false;
    GLOBAL = (Collection){0};
}

//misc
int ZeroRowsCount(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
int FindPivot(int column, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
bool ZeroColumn(int column, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    for (int i = 0; i < m; i++) {
        if (matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}

//general
void GetInput(int *m, int *n) {
    printf("\n");
    printf("Enter values for Matrix %d (m x n): ", GLOBAL.amount + 1);
    scanf(" %d %d", m, n);
    //printf("\n");
}
void GetMatrix(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    printf("\nEnter matrix row by row...\n");
    if (DO_INVERSE) {
        printf("(%d rows, %d columns)\n", m, n / 2);
    } else {
        printf("(%d rows, %d columns)\n", m, n);
    }
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
    GLOBAL.matrices[GLOBAL.amount] = *A;
    GLOBAL.amount++;
    printf("\n");
}
void PrintMatrixRow(int row, int s, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    printf("%c", '|');
    for (int i = 0; i < n; i++) {
        if (AUGMENTED && i == n - 1) {
            printf(" %c ", '|');
            //s += 1;
        }
        if (DO_INVERSE && i == n / 2) {
            printf(" %c ", '|');
        }

        double x = matrix[row][i];
        if (x == (int) x) {
            printf("%*d", s, (int) x);
        } else {
            printf("%*.2lf", s, x);
        }
    }
    printf(" %c", '|');
}
void PrintMatrix(Matrix *A, Matrix *B, Matrix *C) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix1)[100] = (*A).matrix;

    double scale = 3;

    int s = 0;
    for (int i = 0; i < GLOBAL.amount; i++) {
        int temp = FindLargestEntry(&GLOBAL.matrices[i]);
        if (s < temp) {
            s = temp;
        }
    }
    s += scale;

    //

    /*
    for (int i = 0; i < m; i++) {
        PrintMatrixRow(i, s, A);
        if (B != NULL) {
            if (C != NULL) {
                if (i == m / 2) {
                    printf(" %*c ", s - scale, 'X');
                } else {
                    printf(" %*c ", s - scale, ' ');
                }
                PrintMatrixRow(i, s, B);
                if (i == m / 2) {
                    printf(" %*c ", s - scale, '=');
                } else {
                    printf(" %*c ", s - scale, ' ');
                }
                PrintMatrixRow(i, s, C);
            } else {
                PrintMatrixRow(i, s, B);
                if (LU_DONE && i + 1 == m) {
                    printf(" = LU");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
    */
}

//elementary operations
void ScaleRow(int toScale, double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (SHOW_STEPS) {
        printf("R%d —> (%.2lf)R%d\n", toScale + 1, c, toScale + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toScale][i] = (double) matrix[toScale][i] * c;
    }
    if (SHOW_STEPS) {
        PrintMatrix(A, NULL, NULL);
    }
}
void ReplaceRow(int toReplace, int replacer, double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (SHOW_STEPS) {
        printf("R%d —> R%d + (%.1lf)R%d\n", toReplace + 1, toReplace + 1, c, replacer + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toReplace][i] += matrix[replacer][i] * c;
    }
    if (SHOW_STEPS) {
        PrintMatrix(A, NULL, NULL);
    }
}
void SwapRows(int row1, int row2, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
        PrintMatrix(A, NULL, NULL);
    }
}
void ScaleMatrix(int c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (SHOW_STEPS) {
        printf("Scale A by %d\n", c);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= c;
        }
    }
    if (SHOW_STEPS) {
        PrintMatrix(A, NULL, NULL);
    }
}

//algorithm checkers
bool CheckRREF(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
    int zeroRows = ZeroRowsCount(A);
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
bool CheckREF(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
    int zeroRows = ZeroRowsCount(A);
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
bool CheckMult() {
    char decision;
    printf("Multiply matrices? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        DO_MULT = true;
        return true;
    }
    return false;
}

//algorithms
void RREF(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (DO_INVERSE) {
        PrepareInverse(A);
    }
    printf("Gaussian Elimination...\n");
    PrintMatrix(A, NULL, NULL);
    int row = 0;
    int column = 0;
    while (!CheckRREF(A)) {
        if (ZeroColumn(column, A)) {
            column++;
            continue;
        }
        if (matrix[row][column] == 0) {
            int newRow = FindPivot(column, A);
            SwapRows(row, newRow, A);
        }
        if (matrix[row][column] != 1) {
            double c = 1 / matrix[row][column];
            ScaleRow(row, c, A);
        }
        for (int i = 0; i < m; i++) {
            if (i == row) {
                continue;
            }
            if (matrix[i][column] != 0) {
                double c = - matrix[i][column] / matrix[row][column];
                ReplaceRow(i, row, c, A);
            }
        }
        column++;
        row++;
    }
    PrintMatrix(A, NULL, NULL);
}
void REF(Matrix *A, Matrix *B) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrixU)[100] = (*A).matrix;

    bool possibleLU = true;
    printf("Begin REF operations...\n");
    PrintMatrix(A, B, NULL);
    int row = 0;
    int column = 0;
    while (!CheckREF(A)) {
        if (!possibleLU) {
            break;
        }
        if (ZeroColumn(column, A)) {
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
                    double (*matrixL)[100] = (*B).matrix;
                    matrixL[i][row] = - c;
                }
                ReplaceRow(i, row, c, A);
            }
        }
        column++;
        row++;
    }
    PrintMatrix(A, B, NULL);
}
double DET(int m, int n, double matrix[m][n]) {
    if (m != n) {
        return 0;
    }
    return 1;
    //WIP
}

//arithmetic
void MultMatrix(Matrix *A, Matrix *B, Matrix *C) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix1)[100] = (*A).matrix;

    int o = (*B).rows;
    int p = (*B).columns;
    double (*matrix2)[100] = (*B).matrix;

    double (*result)[100] = (*C).matrix;
    ZeroMatrix(C);

    if (n != o) {
        printf("Matrix multiplication is not possible...\n");
    } else {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < p; j++) {
                for (int k = 0; k < n; k++) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        MULT_DONE = true;
        PrintMatrix(A, B, C);
        printf("\n");
    }
}
/*
void AddMatrix(int n, double matrix1[n][n], double matrix2[n][n], double result[n][n]) {
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

bool Start() {
    int m, n;
    GetInput(&m, &n);
    if (m < 1 || n < 1) {
        return false;
    }
    CheckSteps();
    if (CheckAugment()) {
        n++;

        Matrix A;
        A.rows = m;
        A.columns = n;
        GetMatrix(&A);

        RREF(&A);
    } else if (CheckInverse()) {
        if (m != n) {
            DO_INVERSE = false;
        } else {
            n *= 2;

            Matrix A;
            A.rows = m;
            A.columns = n;
            GetMatrix(&A);

            RREF(&A);
        }
    } else if (CheckLU()) {
        if (m != n) {
            DO_LU = false;
        } else {
            Matrix L;
            Matrix U;

            U.rows = m;
            U.columns = n;
            GetMatrix(&U);

            L.rows = m;
            L.columns = n;
            DiagonalOnes(&L);

            REF(&U, &L);
        }
    } else if (CheckMult()) {
        int o, p;
        GetInput(&o, &p);

        Matrix A;
        A.rows = m;
        A.columns = n;

        Matrix B;
        B.rows = o;
        B.columns = p;

        GetMatrix(&A);
        GetMatrix(&B);

        Matrix C;
        C.rows = m;
        C.columns = p;

        MultMatrix(&A, &B, &C);
    } else {
        Matrix A;
        A.rows = m;
        A.columns = n;
        GetMatrix(&A);

        RREF(&A);
    }
    ResetFlags();
    return true;
}

//unused
/*
void ZeroArray(int n, int array[n]) {
    for (int i = 0; i < n; i++) {
        array[i] = 0;
    }
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
bool CheckIdentity(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

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
*/