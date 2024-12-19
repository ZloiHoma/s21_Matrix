#include "s21_helper.h"

int test_zero_negative(int rows, int columns) {
  int result = SUCCESS;
  if (rows <= 0 || columns <= 0) {
    result = FAILURE;
  }
  return result;
}

void remove_rows_columns(matrix_t *A, matrix_t *cut_matrix, int i, int j) {
  int m = 0;
  int n = 0;
  for (int k = 0; k < A->rows; k++) {
    for (int l = 0; l < A->columns; l++) {
      if (k != i && l != j) {
        cut_matrix->matrix[m][n] = A->matrix[k][l];
        n++;
        if (n == cut_matrix->columns) {
          n = 0;
          m++;
        }
      }
    }
  }
}

int compare(double A, double B) {
  int result = SUCCESS;
  char temp_A[100];
  char temp_B[100];
  snprintf(temp_A, sizeof(temp_A), "%.6lf", A);
  snprintf(temp_B, sizeof(temp_B), "%.6lf", B);
  if (strcmp(temp_A, temp_B)) {
    result = FAILURE;
  }
  return result;
}

double get_determinant(matrix_t *A) {
  double res = 0;
  if (A->rows == 1) {
    res = A->matrix[0][0];
  } else if (A->rows == 2) {
    res = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    matrix_t cut_matrix;
    s21_create_matrix(A->rows - 1, A->columns - 1, &cut_matrix);
    res = 0;
    for (int i = 0; i < A->rows; i++) {
      int m = 0;
      for (int k = 1; k < A->rows; k++) {
        int n = 0;
        for (int l = 0; l < A->columns; l++) {
          if (l != i) {
            cut_matrix.matrix[m][n] = A->matrix[k][l];
            n++;
          }
        }
        m++;
      }
      res += (i % 2 == 0 ? 1 : -1) * A->matrix[0][i] *
             get_determinant(&cut_matrix);
    }
    s21_remove_matrix(&cut_matrix);
  }
  return res;
}