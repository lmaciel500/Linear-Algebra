
#ifndef MATRICES_H
#define MATRICES_H

#include <stdbool.h>

typedef enum {
    STANDARD,
    AUGMENTED,
    LU,
    INVERSE,
    REDUCED,
    ECHELON,
    VECTOR
} MatrixType;
typedef struct {
    bool occupied;
    MatrixType type;
    int rows;
    int columns;
    double matrix[50][50];
} Matrix;
typedef enum {
    STORAGE,
    VECTORS,
    PARAMETRIC
} CollectionType;
typedef struct {
    int amount;
    CollectionType type;
    Matrix matrices[26];
} Collection;
typedef struct {
    bool SHOW_STEPS;
    bool AUGMENT;
    bool LU;
    bool INVERSE;
    bool MULT;
    bool REF;
    bool RREF;
} State;

void ReadData(void);
void WriteData(void);
bool Start(void);

#endif