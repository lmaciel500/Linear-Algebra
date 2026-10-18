
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "matrices.h"
#include "functions.h"

extern Collection GLOBAL;
extern State STATE;
extern int matrixSize;

void ZeroMatrix(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}
int FindLargestEntry(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    printf("\n");
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
Collection ChopMatrix(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    Collection vectors = {n, PARAMETRIC};
    char name = 'r';
    for (int i = 0; i < n; i++) {
        Matrix V = {name, true, VECTOR, m, 1};
        ZeroMatrix(&V);
        for (int j = 0; j < m; j++) {
            V.matrix[j][0] = (*A).matrix[j][i];
        }
        vectors.matrices[i] = V;
        name++;
    }
    return vectors;
}

int ZeroRowsCount(Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;
    for (int i = 0; i < m; i++) {
        if (matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}
bool ZeroRow(int row, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;
    for (int i = 0; i < m; i++) {
        if (matrix[row][i] != 0) {
            return false;
        }
    }
    return true;
}
bool CheckZerosBelow(int row, int column, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

    for (int i = row; i < m; i++) {
        if (matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}
bool FindPivotInRow(int row, int *column, Matrix *A) {
    int n = (*A).columns - 1;
    for (int i = 0; i < n; i++) {
        if ((*A).matrix[row][i] == 0) {
            continue;
        }
        if ((*A).matrix[row][i] != 1) {
            return false;
        }
        if ((*A).matrix[row][i] == 1) {
            *column = i;
            return true;
        }
    }
    return false;
}


int GreatestRows(Collection *collection) {
    int greatest = 0;
    for (int i = 0; i < (*collection).amount; i++) {
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


//parametric
bool FindFreeInColumn(int *row, int column, double *value, Matrix *A) {
    for (int i = *row; i < (*A).rows; i++) {
        if ((*A).matrix[i][column] != 0 && (*A).matrix[i][column] != 1) {
            *row = i;
            *value = (*A).matrix[i][column];
            return true;
        }
    }
    return false;
}
bool FindFreeInRow(int row, int *column, double *value, Matrix *A) {
    for (int i = 0; i < (*A).columns - 1; i++) {
        if ((*A).matrix[row][i] != 0 && (*A).matrix[row][i] != 1) {
            *column = i;
            *value = (*A).matrix[row][i];
            return true;
        }
    }
    return false;
}
Collection FindVectors(Matrix *A) {
    Collection vectors;
    vectors.type = VECTORS;
    int rows = (*A).columns - 1;
    vectors.amount = (*A).columns - Rank(A);

    for (int i = 0; i < vectors.amount; i++) {
        vectors.matrices[i].rows = rows;
        vectors.matrices[i].columns = 1;
    }
    for (int i = 0; i < rows; i++) {
        int column;
        if (FindPivotInRow(i, &column, A)) {
            int index = 0;
            for (int j = (*A).columns - 1; j > column; j--) {
                if ((*A).matrix[i][j] != 0) {
                    vectors.matrices[i].matrix[i][j] = (*A).matrix[i][j];
                }
            }
        } else {

        }

    }
    return vectors;
}
Matrix PrepareVectorMatrix(Matrix *A) {
    int rank = Rank(A);
    Matrix B = {0};
    B.rows = (*A).columns - 1;
    B.columns = 1;
    for (int i = 0; i < (*A).rows; i++) {
        B.matrix[i][0] = (*A).matrix[i][(*A).columns - 1];
    }
    int free = 0;
    for (int column = 0; column < (*A).columns - 1; column++) {
        int row = 0;
        double value = 0;
        if (FindFreeInColumn(&row, column, &value, A)) {
            free++;
            B.matrix[column][free] = 1;
            for (int i = row; i < (*A).rows - free; i++) {
                row = i;
                FindFreeInColumn(&row, column, &value, A);
                B.matrix[row][free] = -1 * value;
            }
            B.columns = free + 1;
        }
    }
    return B;
}
void ParametricForm(Matrix *A) {
    if (Rank(A) < (*A).columns - 1 && A->type == AUGMENTED && A->reduced) {
        printf("View parametric vector form?");
        if (Confirm(false)) {
            Matrix B = PrepareVectorMatrix(A);
            Collection parametricForm = ChopMatrix(&B);
            PrintMatrices(&parametricForm);
        }
    }
}


//elementary operations
void ScaleRow(int toScale, double c, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
bool CheckConsistent(Matrix *A) {
    int m = A->rows;
    int n = A->columns - 1;
    if (A->type == AUGMENTED) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (A->matrix[i][j] != 0) {
                    goto nextRow;
                }
            }
            if (A->matrix[i][n] != 0) {
                A->type = INCONSISTENT;
                return false;
            }
            nextRow:
        }
    }
    return true;
}


//rref
int SearchPivotInColumn(int row, int column, Matrix *A) {
    for (int i = row; i < A->rows; i++) {
        if (A->matrix[i][column] != 0) {
            return i;
        }
    }
    return -1;
}
int SearchPivotInRow(int row, int column, Matrix *A) {
    for (int i = column; i < A->columns; i++) {
        if (A->matrix[row][i] != 0) {
            return i;
        }
    }
    return -1;
}
bool ZerosBelow(int row, int column, Matrix *A) {
    for (int i = row + 1; i < A->rows; i++) {
        if (A->matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}
bool ZerosColumn(int row, int column, Matrix *A) {
    for (int i = 0; i < A->rows; i++) {
        if (i == row) {
            continue;
        } else if (A->matrix[i][column] != 0) {
            return false;
        }
    }
    return true;
}
void RREF(Matrix *A) {
    int row = 0;
    int column = 0;

    int m = A->rows;
    int n = A->columns;

    printf("Gaussian Elimination...\n");
    PrintMatrix(A);

    while (row < m && column < n) {
        if (A->matrix[row][column] == 0) {
            if (!ZerosBelow(row, column, A)) {
                int newPivot = SearchPivotInColumn(row, column, A);
                if (newPivot != -1) {
                    SwapRows(row, newPivot, A);
                    column++;
                    continue;
                }
            } else {
                column++;
                continue;
            }
        }
        if (A->matrix[row][column] != 1) {
            double c = 1 / A->matrix[row][column];
            ScaleRow(row, c, A);
        }
        if (!ZerosColumn(row, column, A)) {
            double c;
            for (int i = 0; i < A->rows; i++) {
                if (i == row) {
                    continue;
                }
                c = - A->matrix[i][column] / A->matrix[row][column];
                ReplaceRow(i, row, c, A);
            }
        }
        row++;
        column++;
    }
    if (!CheckConsistent(A)) {
        printf("Inconsistent matrix...\n");
    } else {
        printf("Matrix is in RREF.\n");
    }
    A->reduced = true;
    PrintMatrix(A);
}

void REF(Matrix *A, Matrix *B) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrixU)[matrixSize] = (*A).matrix;
    double (*matrixL)[matrixSize] = (*B).matrix;

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
    if ((*A).type == AUGMENTED) {
        n--;
    }
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
                        goto increase;
                    }
                }
            }
        }
        increase:
    }
    return rank;
}


//operations
void Reduce(int index) {
    PrintLine();
    Matrix R = GLOBAL.matrices[index];
    if (R.type != AUGMENTED) {
        R.type = REDUCED;
    }
    RREF(&R);
    StoreMatrix(&R);
}
void Augment(int index) {
    PrintLine();
    STATE.AUGMENT = true;
    Matrix A = GLOBAL.matrices[index];
    A.type = AUGMENTED;
    AddAugment(&A);
    PrintMatrix(&A);
    StoreMatrix(&A);
    printf("Proceed with row reduction?");
    if (Confirm(false)) {
        RREF(&A);
        StoreMatrix(&A);
    }
    ParametricForm(&A);
}
void Inverse(int index) {
    PrintLine();
    STATE.INVERSE = true;
    Matrix I = GLOBAL.matrices[index];
    I.type = INVERSE;
    PrepareInverse(&I);
    RREF(&I);
    StoreMatrix(&I);
}
void FactorLU(int index) {
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
}
void Multiply(int index) {
    char name = ToChar(index);
    PrintLine();
    printf("Select matrix to multiply with %c (%dx%d)...\n\n",
        name,
        GLOBAL.matrices[index].rows,
        GLOBAL.matrices[index].columns);
    char m = StartSelector();
    if (!Confirm(true)) {
        return;
    }
    if (m == -1) {
        printf("Invalid matrix.\n");
        return;
    }
    int other = ToIndex(m);
    if (GLOBAL.matrices[index].columns == GLOBAL.matrices[other].rows) {
        printf("Multiplying %c with %c...\n", name, m);

        Matrix A = GLOBAL.matrices[index];
        Matrix B = GLOBAL.matrices[other];

        Matrix result = MultiplyMatrices(&A, &B);
        if (result.type == INVALID) {
            return;
        }

        static Collection multiplication;

        multiplication.matrices[0] = A;
        multiplication.matrices[1] = B;
        multiplication.matrices[2] = result;

        multiplication.amount = 3;

        STATE.MULT = true;
        result.type = STANDARD;

        PrintMatrices(&multiplication);
        StoreMatrix(&result);
    }
}
void Scale(int index) {
    char name = ToChar(index);
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
}
void Determinant(int index) {
    PrintLine();
    Matrix D = GLOBAL.matrices[index];
    printf("Det(%c) = %lf\n\n", ToChar(index), Det(&D));
}
void DoRank(int index) {
    char name = ToChar(index);
    PrintLine();
    Matrix C = GLOBAL.matrices[index];
    printf("Rank(%c) = %d\n\n", name, Rank(&C));
}


//arithmetic
Matrix MultiplyMatrices(Matrix *A, Matrix *B) {
    Matrix result;
    if (A->columns != B->rows || (A->type != STANDARD && B->type != STANDARD)) {
        printf("Matrix multiplication is impossible.\n");
        result.type = INVALID;
        return result;
    }
    result.rows = A->columns;
    result.columns = B->rows;

    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix1)[matrixSize] = (*A).matrix;

    int o = (*B).rows;
    int p = (*B).columns;
    double (*matrix2)[matrixSize] = (*B).matrix;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            for (int k = 0; k < n; k++) {
                result.matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    //PrintMatrix();
    printf("\n");
    return result;
}
Matrix AddMatrices(Matrix *A, Matrix *B) {
    Matrix result;
    if (A->rows != B->rows || A->columns != B->columns) {
        printf("Matrix addition is impossible.\n");
        result.type = INVALID;
        return result;
    }
    result.rows = A->rows;
    result.columns = B->columns;
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            result.matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
    }
    return result;
    printf("\n");
}