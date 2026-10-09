
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
        AUGMENT = true;
        n++;
    } else {
        CheckLU();
        if (m != n) {
            DO_LU = false;
        }
    }

    double Umatrix[m][n];
    double Lmatrix[m][n];
    GetMatrix(m, n, Umatrix);
    ZeroMatrix(m, n, Lmatrix);

    REF(m, n, Lmatrix, Umatrix);

    return 0;
}