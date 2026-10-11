#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    STANDARD,
    AUGMENT,
    LU,
    INVERSE,
} MatrixType;
typedef struct {
    bool occupied;
    MatrixType type;
    int rows;
    int columns;
    double matrix[100][100];
} Matrix;
typedef struct {
    int amount;
    Matrix matrices[50];
} Collection;
typedef struct {
    bool SHOW_STEPS;
    bool AUGMENTED;
    bool LU;
    bool INVERSE;
    bool MULT;
} State;

static Collection GLOBAL = {0};
static State STATE = {true, false, false, false, false};

static bool AUGMENTED = false;
static bool SHOW_STEPS = true;
static bool DO_LU = false;
static bool DO_INVERSE = false;
static bool DO_MULT = false;

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
    DO_INVERSE = false;
    DO_MULT = false;
    GLOBAL = (Collection){0};
}
void ResetState() {
    STATE.AUGMENTED = false;
    STATE.LU = false;
    STATE.INVERSE = false;
    STATE.MULT = false;
}

//finders
int ToIndex(char name) {
    return name - 'A';
}
char ToChar(int index) {
    return 'A' + index;
}
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
int GreatestRows() {
    int greatest = 0;
    for (int i = 0; i < GLOBAL.amount; i++) {
        int temp = GLOBAL.matrices[i].rows;
        if (greatest < temp) {
            greatest = temp;
        }
    }
    return greatest;
}
int GreatestRowsNew(Collection *collection) {
    int greatest = 0;
    for (int i = 0; i < GLOBAL.amount; i++) {
        int temp = collection->matrices[i].rows;
        if (greatest < temp) {
            greatest = temp;
        }
    }
    return greatest;
}
int SmallestRows() {
    int smallest = GLOBAL.matrices[0].rows;
    for (int i = 0; i < GLOBAL.amount; i++) {
        int temp = GLOBAL.matrices[i].rows;
        if (smallest > temp) {
            smallest = temp;
        }
    }
    return smallest;
}

bool CheckExists(char name) {
    if (ToIndex(name) >= 0 && ToIndex(name) < GLOBAL.amount) {
        return true;
    } else {
        return false;
    }
}
int FindAvailableIndex() {
    for (int i = 0; i <= GLOBAL.amount; i++) {
        if (!GLOBAL.matrices[i].occupied) {
            return i;
        }
    }
    return -1;
}

//general
void GetInput(int *m, int *n) {
    printf("\n");
    printf("Enter values for Matrix (m x n): ");
    scanf(" %d %d", m, n);
    *m = abs(*m);
    *n = abs(*n);
    if (*m == 0 || *n == 0) {
        *m += 1;
        *n += 1;
    }
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
    GLOBAL.matrices[GLOBAL.amount++] = *A;
    printf("\n");
}
void DeleteMatrix(char name) {
    int index = ToIndex(name);
    GLOBAL.matrices[index].occupied = false;
}
void AddMatrix(Matrix A) {
    int index = FindAvailableIndex();
    if (index == -1) {
        printf("Collection is full.\n");
    } else {
        GLOBAL.matrices[index] = A;
        GLOBAL.matrices[index].occupied = true;
        printf("Created matrix %c.\n", ToChar(index));
        GLOBAL.amount++;
    }
}
void CreateMatrix() {
    int m, n;
    GetInput(&m, &n);
    Matrix A = {0};
    A.rows = m;
    A.columns = n;
    printf("\nEnter matrix row by row...\n");
    printf("(%d rows, %d columns)\n", m, n);
    for (int i = 0; i < m; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf(" %lf", &A.matrix[i][j]);
        }
    }
    AddMatrix(A);
    printf("\n");
}

//matrix printers
void PrintSpacing(int row, int number) {
    int spacing = GreatestRows() - 1;
    if (STATE.MULT) {
        if (row == spacing && number == 0) {
            printf("  X");
        } else if (row == spacing && number == 1) {
            printf("  =");
        } else {
            printf("   ");
        }
    }
}
void PrintMatrixRow(int row, int s, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    printf(" %c", '|');
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
void PrintBlankRow(int s, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    printf(" %c", ' ');
    for (int i = 0; i < n; i++) {
        printf("%*c", s,  ' ');
    }
    printf(" %c", ' ');
}
void PrintMatrixName(char name) {
    Matrix A = GLOBAL.matrices[ToIndex(name)];
    int m = A.rows;
    int n = A.columns;
    double scale = 1;
    int s = FindLargestEntry(&A);
    s += scale;
    for (int i = 0; i < m; i++) {
        PrintMatrixRow(i, s, &A);
        printf("\n");
    }
    printf("\n");
}
void PrintCollection() {
    int offset = 'A';
    if (GLOBAL.amount > 0) {
        for (int i = 0; i < GLOBAL.amount; i++) {
            if (GLOBAL.matrices[i].occupied) {
                int rows = GLOBAL.matrices[i].rows;
                int columns = GLOBAL.matrices[i].columns;
                printf("%c: %dx%d\n", i + offset, rows, columns);
            }
        }
    } else {
        printf("There are no stored matrices.");
    }
    printf("\n");
}
void PrintMatrix() {
    double scale = 1;
    int s = 0;
    for (int i = 0; i < GLOBAL.amount; i++) {
        int temp = FindLargestEntry(&GLOBAL.matrices[i]);
        if (s < temp) {
            s = temp;
        }
    }
    s += scale;

    int height = GreatestRows();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < GLOBAL.amount; j++) {
            int offset = height - GLOBAL.matrices[j].rows;
            if (i < offset) {
                PrintBlankRow(s, &GLOBAL.matrices[j]);
            } else {
                PrintMatrixRow(i - offset, s, &GLOBAL.matrices[j]);
            }
            PrintSpacing(i, j);
        }
        if (i + 1 == height && DO_LU) {
            printf(" = [L][U]");
        } else if (i + 1 == height && DO_INVERSE) {
            printf(" = [ I |A^-1]");
        }
        printf("\n");
    }
    printf("\n");
}
void PrintMatrixNew(Collection *collection) {
    double scale = 1;
    int s = 0;
    for (int i = 0; i < collection->amount; i++) {
        int temp = FindLargestEntry(&collection->matrices[i]);
        if (s < temp) {
            s = temp;
        }
    }
    s += scale;

    int height = GreatestRowsNew(collection);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < collection->amount; j++) {
            int offset = height - collection->matrices[j].rows;
            if (i < offset) {
                PrintBlankRow(s, &collection->matrices[j]);
            } else {
                PrintMatrixRow(i - offset, s, &collection->matrices[j]);
            }
            PrintSpacing(i, j);
        }
        if (i + 1 == height && STATE.LU) {
            printf(" = [L][U]");
        } else if (i + 1 == height && STATE.INVERSE) {
            printf(" = [ I |A^-1]");
        }
        printf("\n");
    }
    printf("\n");
}

//menu printers
void PrintOperations() {
    printf("\n");
    printf("Select operation...\n");
    printf("\n");
    printf("r) Reduced Row Echelon Form\n");
    printf("a) Augmented Matrix\n");
    printf("i) Inverse Matrix\n");
    printf("u) LU Factorization\n");
    printf("m) Matrix Multiplication\n");
    printf("\n");
    printf("Type character (r, a, i, u, m): ");
}
void PrintMenu() {
    printf("\n");
    printf("Main Menu...\n");
    printf("\n");
    printf("c) Create matrix\n");
    printf("v) View stored matrices\n");
    printf("p) Perform operation on matrix\n");
    printf("d) Delete matrix\n");
    printf("\n");
    printf("Type Character (c, v, p, d): ");
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
        PrintMatrix();
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
        PrintMatrix();
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
        PrintMatrix();
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
        PrintMatrix();
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
    return true;
}

//checkers
bool Confirm() {
    printf("Are you sure? (y/n): " );
    char decision;
    scanf(" %c", &decision);
    if (decision == 'y') {
        return true;
    } else {
        return false;
    }
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
    PrintMatrix();
    int row = 0;
    int column = 0;
    while (!CheckRREF(A) && row < m && column < n) {
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
    PrintMatrix();
}
void REF(Matrix *A, Matrix *B) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrixU)[100] = (*A).matrix;

    bool possibleLU = true;
    printf("Begin REF operations...\n");
    PrintMatrix();
    int row = 0;
    int column = 0;
    while (!CheckREF(A) && row < m && column < n) {
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
    PrintMatrix();
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
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    //PrintMatrix();
    printf("\n");
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

char StartSelector() {
    char decision;
    if (GLOBAL.amount > 0) {
        printf("Select matrix: ");
        scanf(" %c", &decision);
        if (CheckExists(decision)) {
            PrintMatrixName(decision);
            return decision;
        } else {
            printf("Matrix does not exist.\n");
            return -1;
        }
    }
    return -1;
}
bool StartOperations(char name) {
    //it somehow crashses before this next print???
    printf("I got here\n");
    int matrix = ToIndex(name);
    PrintOperations();
    char decision;
    scanf(" %c", &decision);
    switch (decision) {
        case 'r':

            break;
        case 'a':

            break;
        case 'i':

            break;
        case 'u':

            break;
        case 'm':
            PrintCollection();
            char m = StartSelector();
            int other = ToIndex(m);
            if (GLOBAL.matrices[matrix].columns == GLOBAL.matrices[other].rows) {
                printf("Multiplying %c with %c...", name, m);

                Matrix A = GLOBAL.matrices[matrix];
                Matrix B = GLOBAL.matrices[other];

                Matrix result;
                result.rows = A.rows;
                result.columns = B.columns;
                ZeroMatrix(&result);

                MultMatrix(&A, &B, &result);
                static Collection multiplication;

                multiplication.matrices[0] = A;
                multiplication.matrices[1] = B;
                multiplication.matrices[2] = result;

                multiplication.amount = 3;

                STATE.MULT = true;

                PrintMatrixNew(&multiplication);
            } else {
                printf("Matrix multiplication is impossible.\n");
            }
            break;
        default:
            break;
            return false;
    }
}
bool Start() {
    char name;
    PrintMenu();
    char decision;
    scanf(" %c", &decision);
    switch (decision) {
        case 'v':
            PrintCollection();
            StartSelector();
            break;
        case 'c':
            CreateMatrix();
            break;
        case 'p':
            PrintCollection();
            name = StartSelector();
            if (CheckExists(name) && Confirm()) {
                //it makes it into this function
                StartOperations(name);
            }
            break;
        case 'd':
            PrintCollection();
            name = StartSelector();
            if (CheckExists(name) && Confirm()) {
                DeleteMatrix(name);
            }
            break;
        default:
            break;
    }
    ResetState();
    return true;
}

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
