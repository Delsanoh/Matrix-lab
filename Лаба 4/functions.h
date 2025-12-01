#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <cstdint>

// Прототипы функций
void printMatrix(const std::vector<std::vector<double>>& matrix);
std::vector<std::vector<double>> createMatrix(int32_t n);
std::vector<std::vector<double>> inputMatrixKeyboard();
std::vector<std::vector<double>> inputMatrixRandom();
std::vector<std::vector<double>> inputMatrix();
void findMaxInSubmatrix(const std::vector<std::vector<double>>& matrix, int32_t start, int32_t& maxRow, int32_t& maxCol);
void swapRows(std::vector<std::vector<double>>& matrix, int32_t row1, int32_t row2);
void swapColumns(std::vector<std::vector<double>>& matrix, int32_t col1, int32_t col2);
bool transformMatrix(std::vector<std::vector<double>>& matrix);
int32_t findFirstRowWithoutPositive(const std::vector<std::vector<double>>& matrix);
void checkMainDiagonal(const std::vector<std::vector<double>>& matrix);

#endif 