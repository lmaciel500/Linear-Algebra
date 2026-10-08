
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "matrices.h"

int main(void) {
    printf("\n");
    int m, n;
    GetInput(&m, &n);

    if (CheckAugment()) {
        AUGMENT = true;
        n++;
    }
    CheckSteps();
    CheckLU();

    double matrix1[m][n];
    GetMatrix(m, n, matrix1);

    REF(m, n, matrix1);

    return 0;
}