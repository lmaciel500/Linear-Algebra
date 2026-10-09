
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "matrices.h"

int main(void) {
    printf("\n");
    int m, n;
    GetInput(&m, &n);

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
            GetMatrix(m, n, matrixU);
            REF(m, n, matrixL, matrixU);
        }
    } else {
        double matrix[m][n];
        GetMatrix(m, n, matrix);
        RREF(m, n, matrix);
    }
    return 0;
}