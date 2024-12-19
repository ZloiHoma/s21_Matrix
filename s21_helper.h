#ifndef S21_MATRIX
#define S21_MATRIX

#include "s21_matrix.h"

int test_zero_negative(int rows, int columns);
void remove_rows_columns(matrix_t *A, matrix_t *cut_matrix, int i, int j);
int compare(double A, double B);
double get_determinant(matrix_t *A);

#endif