
#ifndef LINEAR_ALGEBRA_PROTOTYPE_H
#define LINEAR_ALGEBRA_PROTOTYPE_H

#include <stdbool.h>
#include "matrices.h"

typedef struct newMatrix {
    char name;
    bool occupied;
    bool reduced;
    MatrixType type;
    int rows;
    int columns;
    double **data;
} newMatrix;

#endif //LINEAR_ALGEBRA_PROTOTYPE_H