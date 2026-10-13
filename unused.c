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