
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

    double matrix1[m][n];
    GetMatrix(m, n, matrix1);

    //ScaleMatrix(2, n, matrix1);
    //CheckIdentity(n, matrix1);

    RREF(m, n, matrix1);

    return 0;
}