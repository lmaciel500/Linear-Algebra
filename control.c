
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include <windows.h>
#include <unistd.h>

#include "matrices.h"
#include "functions.h"

Collection GLOBAL = {0};
State STATE = {true, false, false, false, false, false, false};
int matrixSize = 50;


//communicators
int ToIndex(char name) {
    return name - 'A';
}
char ToChar(int index) {
    return index + 'A';
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
Matrix CreateMatrix() {
    PrintLine();
    int m, n;
    GetInput(&m, &n);
    Matrix A = {0};
    A.rows = m;
    A.columns = n;
    printf("\nEnter matrix row by row...\n");
    printf("(%d rows, %d columns)\n\n", m, n);
    for (int i = 0; i < m; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < n; j++) {
            scanf(" %lf", &A.matrix[i][j]);
        }
    }
    printf("\n");
    return A;
}
int AddMatrix(Matrix A) {
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
void DeleteMatrix(char name) {
    int index = ToIndex(name);
    GLOBAL.matrices[index].occupied = false;
}
void StoreMatrix(Matrix *A) {
    char decision;
    printf("Store this matrix? (y/n): ");
    scanf(" %c", &decision);
    if (decision == 'y') {
        AddMatrix(*A);
    }
}


//printers
void PrintSpacing(int row, int number, char *parameter, Collection *collection) {
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
    if ((*collection).type == PARAMETRIC) {
        if (row == spacing && number < (*collection).amount - 1) {
            printf("  + %c", *parameter);
            (*parameter)++;
        } else {
            printf("     ");
        }
    }
}
void PrintMatrixRow(int row, int s, Matrix *A) {
    int m = (*A).rows;
    int n = (*A).columns;
    double (*matrix)[matrixSize] = (*A).matrix;

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
    double (*matrix)[matrixSize] = (*A).matrix;

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
    char parameter = 's';
    int height = GreatestRows(collection);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < collection->amount; j++) {
            int offset = height - collection->matrices[j].rows;
            if (i < offset) {
                PrintBlankRow(s, &collection->matrices[j]);
            } else {
                PrintMatrixRow(i - offset, s, &collection->matrices[j]);
            }
            PrintSpacing(i, j, &parameter, collection);
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
                        break;
                }
                printf("%s", type);
                printf("\n");
            }
        }
    } else {
        printf("There are no stored matrices.\n");
    }
    printf("\n");
}

void PrintLine() {
    int n = 20;
    for (int i = 0; i < n; i++) {
        printf("~");
    }
    printf("\n");
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
void PrintOperations() {
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
void PrintSettings() {
    printf("Settings...\n");
    printf("\n");
    printf("o) Show Steps = %s\n", STATE.SHOW_STEPS ? "True" : "False");
    printf("d) Save Data\n");
    printf("d) Delete All\n");
    printf("\n");
    printf("Type Character: ");
}


//logic
bool Confirm(bool ask) {
    bool confirm;
    if (ask) {
        printf("Are you sure?");
    }
    printf(" (y/n): ");
    char decision;
    scanf(" %c", &decision);
    if (decision == 'y') {
        confirm = true;
    } else {
        confirm = false;
    }
    printf("\n");
    return confirm;
}
void ResetState() {
    STATE.AUGMENT = false;
    STATE.LU = false;
    STATE.INVERSE = false;
    STATE.MULT = false;
    STATE.REF = false;
    STATE.RREF = false;
}
char SelectMenu() {
    char decision;
    scanf(" %c", &decision);
    printf("\n");
    return decision;
}

//main logic
void StartSettings() {
    PrintLine();
    PrintSettings();
    switch (SelectMenu()) {
        case 'o':
            STATE.SHOW_STEPS = !STATE.SHOW_STEPS;
            printf("Show Steps set to %s.\n\n", STATE.SHOW_STEPS ? "true" : "false");
            break;
        case 's':
            WriteData();
            break;
        case 'd':
            if (Confirm(true)) {
                WipeData();
            }
        default:
            break;
    }
}
char StartSelector() {
    PrintLine();
    PrintCollection();
    if (GLOBAL.amount > 0) {
        printf("Select matrix: ");
        char decision = SelectMenu();
        if (CheckExists(decision)) {
            Matrix A = GLOBAL.matrices[ToIndex(decision)];
            PrintMatrix(&A);
            return decision;
        } else {
            printf("Matrix does not exist.\n\n");
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
    switch (SelectMenu()) {
        case 'r':
            Reduce(index);
            break;
        case 'a':
            Augment(index);
            break;
        case 'i':
            Inverse(index);
            break;
        case 'u':
            FactorLU(index);
            break;
        case 'm':
            Multiply(index);
            break;
        case 's':
            Scale(index);
            break;
        case 'd':
            Determinant(index);
            break;
        case 'c':
            DoRank(index);
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
    switch (SelectMenu()) {
        case 'v':
            char name = StartSelector();
            if (name == -1) {
                return true;
            }
            Matrix A = GLOBAL.matrices[ToIndex(name)];
            if (A.type == AUGMENTED) {
                ParametricForm(&A);
            }
            break;
        case 'c':
            Matrix N = CreateMatrix();
            AddMatrix(N);
            break;
        case 'p':
            name = StartSelector();
            if (CheckExists(name) && Confirm(true)) {
                //it makes it into this function
                while (StartOperations(name));
            }
            break;
        case 'd':
            name = StartSelector();
            if (CheckExists(name) && Confirm(true)) {
                DeleteMatrix(name);
            }
            break;
        case 's':
            StartSettings();
            break;
        case '/':
            WriteData();
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
    printf("\n");
    printf("Reading data from disk...\n\n");
    sleep(1);
    FILE* inFile = fopen("data.txt", "r");
    if (inFile == NULL) {
        printf("Error reading data from disk.\n");
        return;
    }
    fscanf(inFile, "%d ", &GLOBAL.amount);
    if (GLOBAL.amount == 0) {
        printf("There are no stored matrices.\n\n");
        sleep(1);
    }
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
    printf("\n");
    printf("Writing data to disk...\n\n");
    sleep(1);
    FILE* inFile = fopen("data.txt", "w");
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
void WipeData() {
    sleep(1);
    for (int k = 0; k < GLOBAL.amount; k++) {
        if (GLOBAL.matrices[k].occupied) {
            GLOBAL.matrices[k].occupied = false;
        }
    }
    GLOBAL.amount = 0;
    printf("Data has been wiped.\n\n");
}