#include "s21_matrix.h"

#include "s21_helper.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  enum error status = OK;

  if (!test_zero_negative(rows, columns) || result == NULL) {
    return status = ERROR_INCORRECT_MATRIX;
  }

  result->columns = columns;
  result->rows = rows;

  result->matrix = (double **)malloc(rows * sizeof(double *));
  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double *)malloc(columns * sizeof(double));
  }
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = 0;
    }
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    if (test_zero_negative(A->rows, A->columns)) {
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);
      A->matrix = NULL;
      A->rows = 0;
      A->columns = 0;
    }
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = SUCCESS;
  if (A == NULL && B == NULL) {
    if (A == NULL || B == NULL) {
      result = SUCCESS;
    } else {
      result = FAILURE;
    }
  }
  if (result == SUCCESS && (A->matrix == NULL || B->matrix == NULL)) {
    result = FAILURE;
  }
  if (result == SUCCESS && (A->columns != B->columns || A->rows != B->rows)) {
    result = FAILURE;
  }
  if (result == SUCCESS) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (!compare(A->matrix[i][j], B->matrix[i][j])) {
          result = FAILURE;
          break;
        }
      }
      if (result == FAILURE) break;
    }
  }
  return result;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error res = OK;
  if (res == OK &&
      ((A == NULL || B == NULL) || (A->matrix == NULL || B->matrix == NULL) ||
       result == NULL)) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && (!test_zero_negative(A->rows, A->columns) ||
                    !test_zero_negative(B->rows, B->columns))) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && (A->rows != B->rows || A->columns != B->columns)) {
    res = CALCULATION_ERROR;
  }
  if (res == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error res = OK;
  if (res == OK &&
      ((A == NULL || B == NULL) || (A->matrix == NULL || B->matrix == NULL) ||
       result == NULL)) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && (A->rows != B->rows || A->columns != B->columns)) {
    res = CALCULATION_ERROR;
  }
  if (res == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  enum error res = OK;
  if (A == NULL || A->matrix == NULL ||
      !test_zero_negative(A->rows, A->columns)) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && ((number != number))) {
    res = CALCULATION_ERROR;
  }
  if (res == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error res = OK;
  if (res == OK &&
      ((A == NULL || B == NULL) || (A->matrix == NULL || B->matrix == NULL) ||
       result == NULL)) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && (!test_zero_negative(A->rows, A->columns) ||
                    !test_zero_negative(B->rows, B->columns))) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && A->columns != B->rows) {
    res = CALCULATION_ERROR;
  }
  if (res == OK) {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  enum error res = OK;
  if (res == OK && (A == NULL || result == NULL || A->matrix == NULL ||
                    !test_zero_negative(A->rows, A->columns))) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK) {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->rows != A->columns) {
    status = CALCULATION_ERROR;
  }
  if (!test_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->rows == 1) {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->matrix[0][0] != 0) {
      result->matrix[0][0] = 1 / A->matrix[0][0];
    } else {
      status = CALCULATION_ERROR;
    }
  }
  if (A->rows > 1) {
    double verificare = 0;
    s21_determinant(A, &verificare);
    if (verificare == 0) {
      status = CALCULATION_ERROR;
    } else {
      s21_create_matrix(A->rows, A->columns, result);
      matrix_t temp_complements = {0};
      matrix_t temp_transpose = {0};
      s21_create_matrix(A->rows, A->columns, &temp_complements);
      s21_create_matrix(A->rows, A->columns, &temp_transpose);
      s21_calc_complements(A, &temp_complements);
      s21_transpose(&temp_complements, &temp_transpose);
      s21_mult_number(&temp_transpose, -1, result);
      s21_remove_matrix(&temp_complements);
      s21_remove_matrix(&temp_transpose);
    }
  }
  return status;
}

int s21_determinant(matrix_t *A, double *result) {
  enum error res = OK;
  if (res == OK && (A == NULL || result == NULL || A->matrix == NULL ||
                    !test_zero_negative(A->rows, A->columns))) {
    res = ERROR_INCORRECT_MATRIX;
  }
  if (res == OK && A->rows != A->columns) {
    res = CALCULATION_ERROR;
  }
  if (res == OK) {
    *result = get_determinant(A);
  }
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = OK;
  if (A == NULL || result == NULL || A->matrix == NULL ||
      !test_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  } else if (A->rows != A->columns) {
    status = CALCULATION_ERROR;
  } else {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->rows == 1) {
      result->matrix[0][0] = A->matrix[0][0];
    } else {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          matrix_t minor = {0};
          s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
          remove_rows_columns(A, &minor, i, j);
          double det = 0;
          s21_determinant(&minor, &det);
          s21_remove_matrix(&minor);
          if (fabs(det) > 1e-6) {
            if ((i + j) % 2 == 0) {
              result->matrix[i][j] = det;
            } else {
              result->matrix[i][j] = -det;
            }
          } else {
            result->matrix[i][j] = 0;
          }
        }
      }
    }
  }
  return status;
}