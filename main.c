
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
    } else if (CheckInverse()) {
        if (m != n) {
            DO_INVERSE = false;
        } else {
            n *= 2;
        }
    } else if (CheckLU()) {
        if (m != n) {
            DO_LU = false;
        }
    }

    double Umatrix[m][n];
    double Lmatrix[m][n];

    GetMatrix(m, n, Umatrix);
    ZeroMatrix(m, n, Lmatrix);

    RREF(m, n, Umatrix);

    return 0;
}