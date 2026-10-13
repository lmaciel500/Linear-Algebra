#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "matrices.h"

Collection GLOBAL = {0};
static State STATE = {true, false, false, false, false, false, false};


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
    (*A).columns *= 2;
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
void AddAugment(Matrix *A) {
    int m = (*A).rows;
    double augment[m];
    printf("Enter augment...\n");
    printf("(%d values): ", m);
    for (int i = 0; i < m; i++) {
        scanf(" %lf", &augment[i]);
    }
    (*A).columns++;
    for (int i = 0; i < m; i++) {
        (*A).matrix[i][(*A).columns - 1] = augment[i];
    }
}
Matrix CombineMatrix(Matrix *A, Matrix *B) {
    Matrix combined;
    int columns = (*A).columns + (*B).columns;
    int rows = (*A).rows;
    combined.rows = rows;
    combined.columns = columns;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < (*A).columns; j++) {
            combined.matrix[i][j] = (*A).matrix[i][j];
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = (*A).columns; j < columns; j++) {
            combined.matrix[i][j] = (*B).matrix[i][j - (*A).columns];
        }
    }
    return combined;
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
Collection FindVectors(Matrix *A) {
    Collection vectors;
    return vectors;
}

int GreatestRows(Collection *collection) {
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
    if (ToIndex(name) >= 0 && ToIndex(name) < GLOBAL.amount && GLOBAL.matrices[ToIndex(name)].occupied) {
        return true;
    } else {
        return false;
    }
}
int FindAvailableIndex() {
    for (int i = 0; i < 50; i++) {
        if (!GLOBAL.matrices[i].occupied) {
            return i;
        }
    }
    return -1;
}


//printers
void PrintSpacing(int row, int number, Collection *collection) {
    int spacing = GreatestRows(collection) - 1;
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
        if ((*A).type == AUGMENTED && i == n - 1) {
            printf(" %c ", '|');
            //s += 1;
        }
        if ((*A).type == INVERSE && i == n / 2) {
            printf(" %c ", '|');
        }
        if ((*A).type == LU && i == n / 2) {
            printf(" %c", '|');
            printf(" %c", '|');
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

void PrintMatrices(Collection *collection) {
    double scale = 1;
    int s = 0;
    for (int i = 0; i < collection->amount; i++) {
        int temp = FindLargestEntry(&collection->matrices[i]);
        if (s < temp) {
            s = temp;
        }
    }
    s += scale;

    int height = GreatestRows(collection);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < collection->amount; j++) {
            int offset = height - collection->matrices[j].rows;
            if (i < offset) {
                PrintBlankRow(s, &collection->matrices[j]);
            } else {
                PrintMatrixRow(i - offset, s, &collection->matrices[j]);
            }
            PrintSpacing(i, j, collection);
        }
        if (i + 1 == height && STATE.LU) {
            printf(" = [L][U]");
        }
        printf("\n");
    }
    printf("\n");
}
void PrintMatrix(Matrix *A) {
    int m = A->rows;
    int n = A->columns;
    double scale = 1;
    int s = FindLargestEntry(A);
    s += scale;
    for (int i = 0; i < m; i++) {
        PrintMatrixRow(i, s, A);
        if (STATE.INVERSE && i == m - 1 && STATE.RREF) {
            printf(" = [ I | A`]\n");
        } else {
            printf("\n");
        }
    }
    printf("\n");
}
void PrintCollection() {
    char types[4][20] = {
        "(RREF)",
        "(Augmented)",
        "(Inverse)",
        "(LU Factorization)"
    };
    int offset = 'A';
    if (GLOBAL.amount > 0) {
        for (int i = 0; i < GLOBAL.amount; i++) {
            if (GLOBAL.matrices[i].occupied) {
                int rows = GLOBAL.matrices[i].rows;
                int columns = GLOBAL.matrices[i].columns;
                printf("%c: %dx%d ", i + offset, rows, columns);
                char type[20] = "";
                switch (GLOBAL.matrices[i].type) {
                    case REDUCED:
                        strcpy(type, types[0]);
                        break;
                    case AUGMENTED:
                        strcpy(type, types[1]);
                        break;
                    case INVERSE:
                        strcpy(type, types[2]);
                        break;
                    case LU:
                        strcpy(type, types[3]);
                        break;
                    default:
                        //not sure if I need this here lol
                        break;
                }
                printf("%s", type);
                printf("\n");
            }
        }
    } else {
        printf("There are no stored matrices.");
    }
    printf("\n");
}

void PrintOperations() {
    printf("\n");
    printf("Select operation...\n");
    printf("\n");
    printf("r) Reduced Row Echelon Form\n");
    printf("a) Augmented Matrix\n");
    printf("i) Inverse Matrix\n");
    printf("u) LU Factorization\n");
    printf("m) Matrix Multiplication\n");
    printf("s) Scale Matrix\n");
    printf("d) Determinant\n");
    printf("c) Rank\n");
    printf("\n");
    printf("Type character: ");
}
void PrintMenu() {
    printf("Main Menu...\n");
    printf("\n");
    printf("c) Create matrix\n");
    printf("v) View stored matrices\n");
    printf("p) Perform operation on matrix\n");
    printf("d) Delete matrix\n");
    printf("s) Settings\n");
    printf("\n");
    printf("Type Character: ");
}
void PrintSettings() {
    printf("Settings...\n");
    printf("\n");
    printf("s) Show Steps = %s\n", STATE.SHOW_STEPS ? "True" : "False");
    printf("\n");
    printf("Type Character: ");
}
void PrintLine() {
    int n = 20;
    for (int i = 0; i < n; i++) {
        printf("~");
    }
    printf("\n");
}


//general
void GetInput(int *m, int *n) {
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
        printf("Created matrix %c.\n\n", ToChar(index));
        PrintMatrix(&A);
        GLOBAL.amount++;
    }
}
void CreateMatrix() {
    PrintLine();
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


//elementary operations
void ScaleRow(int toScale, double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (STATE.SHOW_STEPS) {
        printf("R%d —> (%.2lf)R%d\n", toScale + 1, c, toScale + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toScale][i] = (double) matrix[toScale][i] * c;
    }
    if (STATE.SHOW_STEPS) {
        PrintMatrix(A);
    }
}
void ReplaceRow(int toReplace, int replacer, double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (STATE.SHOW_STEPS) {
        printf("R%d —> R%d + (%.1lf)R%d\n", toReplace + 1, toReplace + 1, c, replacer + 1);
        //getchar();
    }
    for (int i = 0; i < n; i++) {
        matrix[toReplace][i] += matrix[replacer][i] * c;
    }
    if (STATE.SHOW_STEPS) {
        PrintMatrix(A);
    }
}
void SwapRows(int row1, int row2, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    if (STATE.SHOW_STEPS) {
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
    if (STATE.SHOW_STEPS) {
        PrintMatrix(A);
    }
}
void ScaleMatrix(double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= c;
        }
    }
    if (STATE.SHOW_STEPS) {
        PrintMatrix(A);
    }
}


//algorithms
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
    return true;
}

void RREF(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[100] = (*A).matrix;
    printf("Gaussian Elimination...\n");
    PrintMatrix(A);
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
    printf("Matrix is in RREF.\n");
    PrintMatrix(A);
}
void REF(Matrix *A, Matrix *B) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrixU)[100] = (*A).matrix;
    double (*matrixL)[100] = (*B).matrix;

    bool possibleLU = true;
    printf("Begin REF operations...\n");
    PrintMatrix(A);
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
            if (STATE.LU && matrixU[row][column] == 0 && matrixU[i][column] != 0) {
                printf("REF for LU impossible...\n");
                possibleLU = false;
                break;
            }
            if (matrixU[i][column] != 0) {
                double c = - matrixU[i][column] / matrixU[row][column];
                if (STATE.LU) {
                    matrixL[i][row] = - c;
                }
                ReplaceRow(i, row, c, A);
            }
        }
        column++;
        row++;
    }
    static Collection L_U;
    L_U.matrices[0] = *B;
    L_U.matrices[1] = *A;
    L_U.amount = 2;
    printf("Matrix is in REF.\n");
    PrintMatrices(&L_U);
}
double Det(Matrix *A) {
	double determinant = 0;

	int m = (*A).rows;
	int n = (*A).columns;

    if (m != n) {
		printf("Matrix must be square.\n");
		return 0;
    }

	if (m == 2) {
		determinant = (*A).matrix[0][0] * (*A).matrix[1][1] - (*A).matrix[0][1] * (*A).matrix[1][0];
	} else {
		double coefficients[n];
		for(int i = 0; i < n; i++) {
			coefficients[i] = (*A).matrix[0][i];
		}
		for (int excluded = 0; excluded < n; excluded++) {
			Matrix B = {0};
			B.rows = m - 1;
			B.columns = n - 1;

			int o = B.rows;
			int p = B.columns;
			for (int i = 0; i < o; i++) {
				for (int j = 0; j < p; j++) {
					int shift = 0;
					if (!(j < excluded)) {
						shift++;
					}
					B.matrix[i][j] = (*A).matrix[i + 1][j + shift];
				}
			}
			if (excluded % 2 == 0) {
				determinant += coefficients[excluded] * Det(&B);
			} else {
				determinant -= coefficients[excluded] * Det(&B);
			}
		}
	}
    return determinant;
}
int Rank(Matrix *A) {
    int rank = 0;
    int m = (*A).rows;
    int n = (*A).columns;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if ((*A).matrix[i][j] == 1) {
                for (int k = 0; k < m; k++) {
                    if (k == i) {
                        continue;
                    }
                    if ((*A).matrix[k][j] != 0) {
                        break;
                    }
                    if (k == m - 1) {
                        rank++;
                    }
                }
            }
        }
    }
    return rank;
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


//logic
void StoreMatrix(Matrix *A) {
    char decision;
    printf("Store this matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        AddMatrix(*A);
    }
}
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
void ResetState() {
    STATE.AUGMENT = false;
    STATE.LU = false;
    STATE.INVERSE = false;
    STATE.MULT = false;
    STATE.REF = false;
    STATE.RREF = false;
}


void StartSettings() {
    PrintLine();
    PrintSettings();
    char decision;
    scanf(" %c", &decision);
    switch (decision) {
        case 's':
            STATE.SHOW_STEPS = !STATE.SHOW_STEPS;
            printf("Show Steps set to %s.\n\n", STATE.SHOW_STEPS ? "true" : "false");
            break;
        default:
            break;
    }
}
char StartSelector() {
    PrintLine();
    PrintCollection();
    char decision;
    if (GLOBAL.amount > 0) {
        printf("Select matrix: ");
        scanf(" %c", &decision);
        if (CheckExists(decision)) {
            Matrix A = GLOBAL.matrices[ToIndex(decision)];
            PrintMatrix(&A);
            return decision;
        } else {
            printf("Matrix does not exist.\n");
            return -1;
        }
    }
    return -1;
}
bool StartOperations(char name) {
    PrintLine();
    printf("Current matrix: %c\n\n", name);
    int index = ToIndex(name);
    //PrintSeparator(&GLOBAL.matrices[index]);
    PrintMatrix(&GLOBAL.matrices[index]);
    PrintOperations();
    char decision;
    scanf(" %c", &decision);
    switch (decision) {
        case 'r':
            PrintLine();
            Matrix R = GLOBAL.matrices[index];
            R.type = REDUCED;
            RREF(&R);
            StoreMatrix(&R);
            break;
        case 'a':
            PrintLine();
            STATE.AUGMENT = true;
            Matrix A = GLOBAL.matrices[index];
            A.type = AUGMENTED;
            AddAugment(&A);
            RREF(&A);
            StoreMatrix(&A);
            break;
        case 'i':
            PrintLine();
            STATE.INVERSE = true;
            Matrix I = GLOBAL.matrices[index];
            I.type = INVERSE;
            PrepareInverse(&I);
            RREF(&I);
            StoreMatrix(&I);
            break;
        case 'u':
            PrintLine();
            STATE.LU = true;
            Matrix L;
            L.rows = GLOBAL.matrices[index].rows;
            L.columns = GLOBAL.matrices[index].columns;
            DiagonalOnes(&L);
            Matrix U = GLOBAL.matrices[index];
            REF(&U, &L);
            Matrix combinedLU = CombineMatrix(&L, &U);
            combinedLU.type = LU;
            StoreMatrix(&combinedLU);
            break;
        case 'm':
            PrintLine();
            printf("Select matrix to multiply with %c (%dx%d)...\n\n",
                name,
                GLOBAL.matrices[index].rows,
                GLOBAL.matrices[index].columns);
            char m = StartSelector();
            if (!Confirm()) {
                break;
            }
            if (m == -1) {
                printf("Invalid matrix.\n");
                break;
            }
            int other = ToIndex(m);
            if (GLOBAL.matrices[index].columns == GLOBAL.matrices[other].rows) {
                printf("Multiplying %c with %c...", name, m);

                Matrix A = GLOBAL.matrices[index];
                Matrix B = GLOBAL.matrices[other];

                Matrix result;
                result.type = STANDARD;
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
                result.type = STANDARD;

                PrintMatrices(&multiplication);
                StoreMatrix(&result);
            } else {
                printf("Matrix multiplication is impossible.\n");
            }
            break;
        case 's':
            PrintLine();
            Matrix S = GLOBAL.matrices[index];
            double scalar;
            printf("Enter scalar (c): ");
            scanf(" %lf", &scalar);
            if (STATE.SHOW_STEPS) {
                printf("Scaled %c by %.2lf\n", name, scalar);
            }
            ScaleMatrix(scalar, &S);
            StoreMatrix(&S);
            break;
        case 'd':
            PrintLine();
            Matrix D = GLOBAL.matrices[index];
            printf("Det(%c) = %lf\n\n", ToChar(index), Det(&D));
            break;
        case 'c':
            PrintLine();
            Matrix C = GLOBAL.matrices[index];
            printf("Rank(%c) = %d\n\n", name, Rank(&C));
            break;
        default:
            return false;
            break;
    }
    ResetState();
    return true;
}


bool Start() {
    PrintLine();
    char name;
    PrintMenu();
    char decision;
    scanf(" %c", &decision);
    switch (decision) {
        case 'v':
            StartSelector();
            break;
        case 'c':
            CreateMatrix();
            break;
        case 'p':
            name = StartSelector();
            if (CheckExists(name) && Confirm()) {
                //it makes it into this function
                while (StartOperations(name));
            }
            break;
        case 'd':
            name = StartSelector();
            if (CheckExists(name) && Confirm()) {
                DeleteMatrix(name);
            }
            break;
        case 's':
            StartSettings();
            break;
        default:
            return false;
            break;
    }
    ResetState();
    return true;

}


//data writing and parsing
void ReadData() {
    printf("Reading data from disk...\n\n");
    Sleep(1000);
    FILE* inFile = fopen("C:\\Users\\Luis Maciel\\Documents\\CLion\\C\\Linear-Algebra\\data.txt", "r");
    if (inFile == NULL) {
        printf("Error reading data from disk.\n");
        return;
    }
    fscanf(inFile, "%d ", &GLOBAL.amount);
    int steps;
    fscanf(inFile, "%d\n", &steps);
    if (steps) {
        STATE.SHOW_STEPS = true;
    } else {
        STATE.SHOW_STEPS = false;
    }
    for (int k = 0; k < GLOBAL.amount; k++) {
        Matrix A;
        int occupied;
        fscanf(inFile, "%d ", &occupied);
        if (occupied) {
            A.occupied = true;
        } else {
            A.occupied = false;
        }
        char type;
        fscanf(inFile, "%c ", &type);
        switch (type) {
            case 'r':
                A.type = REDUCED;
                break;
            case 'a':
                A.type = AUGMENTED;
                break;
            case 'i':
                A.type = INVERSE;
                break;
            case 'u':
                A.type = LU;
                break;
            default:
                A.type = STANDARD;
                break;
        }
        int rows, columns;
        fscanf(inFile, "%d ", &rows);
        fscanf(inFile, "%d ", &columns);

        A.rows = rows;
        A.columns = columns;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                double temp;
                fscanf(inFile, "%lf ", &temp);
                A.matrix[i][j] = temp;
            }
        }
        GLOBAL.matrices[k] = A;
        fscanf(inFile, "\n");
    }
    fclose(inFile);
}
void WriteData() {
    printf("Writing data to disk...\n");
    Sleep(1000);
    FILE* inFile = fopen("C:\\Users\\Luis Maciel\\Documents\\CLion\\C\\Linear-Algebra\\data.txt", "w");
    if (inFile == NULL) {
        printf("Error writing data to disk.\n");
        return;
    }
    fprintf(inFile, "%d ", GLOBAL.amount);
    fprintf(inFile, "%d\n", STATE.SHOW_STEPS);
    for (int k = 0; k < GLOBAL.amount; k++) {
        if (GLOBAL.matrices[k].occupied) {
            fprintf(inFile, "%d ", 1);
        } else {
            fprintf(inFile, "%d ", 0);
        }
        switch (GLOBAL.matrices[k].type) {
            case REDUCED:
                fprintf(inFile, "%c ", 'r');
                break;
            case AUGMENTED:
                fprintf(inFile, "%c ", 'a');
                break;
            case INVERSE:
                fprintf(inFile, "%c ", 'i');
                break;
            case LU:
                fprintf(inFile, "%c ", 'u');
                break;
            default:
                fprintf(inFile, "%c ", 's');
                break;
        }
        fprintf(inFile, "%d ", GLOBAL.matrices[k].rows);
        fprintf(inFile, "%d ", GLOBAL.matrices[k].columns);
        for (int i = 0; i < GLOBAL.matrices[k].rows; i++) {
            for (int j = 0; j < GLOBAL.matrices[k].columns; j++) {
                fprintf(inFile, "%lf ", GLOBAL.matrices[k].matrix[i][j]);
            }
        }
        fprintf(inFile, "\n");
    }
    fclose(inFile);
}