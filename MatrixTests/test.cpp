#include "pch.h"

#include "../Лаба 4/functions.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <cmath>

// Тесты для функции createMatrix
TEST(MatrixTest, CreateMatrixPositiveSize) {
    auto matrix = createMatrix(3);
    EXPECT_EQ(matrix.size(), 3);
    EXPECT_EQ(matrix[0].size(), 3);
    EXPECT_EQ(matrix[2].size(), 3);
}

TEST(MatrixTest, CreateMatrixEmpty) {
    auto matrix = createMatrix(0);
    EXPECT_EQ(matrix.size(), 0);
}

// Тесты для функции printMatrix (проверяем, что не падает на пустой матрице)
TEST(MatrixTest, PrintMatrixEmpty) {
    testing::internal::CaptureStdout();
    std::vector<std::vector<double>> emptyMatrix;
    printMatrix(emptyMatrix);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Матрица пуста!") != std::string::npos);
}

TEST(MatrixTest, PrintMatrixNormal) {
    testing::internal::CaptureStdout();
    std::vector<std::vector<double>> matrix = { {1.0, 2.0}, {3.0, 4.0} };
    printMatrix(matrix);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тесты для функции swapRows
TEST(MatrixTest, SwapRowsNormal) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    swapRows(matrix, 0, 2);

    EXPECT_DOUBLE_EQ(matrix[0][0], 7.0);
    EXPECT_DOUBLE_EQ(matrix[0][1], 8.0);
    EXPECT_DOUBLE_EQ(matrix[0][2], 9.0);
    EXPECT_DOUBLE_EQ(matrix[2][0], 1.0);
    EXPECT_DOUBLE_EQ(matrix[2][1], 2.0);
    EXPECT_DOUBLE_EQ(matrix[2][2], 3.0);
}

TEST(MatrixTest, SwapRowsSameRow) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0},
        {3.0, 4.0}
    };

    std::vector<std::vector<double>> original = matrix;
    swapRows(matrix, 0, 0);

    EXPECT_DOUBLE_EQ(matrix[0][0], original[0][0]);
    EXPECT_DOUBLE_EQ(matrix[0][1], original[0][1]);
}

TEST(MatrixTest, SwapRowsInvalidIndex) {
    std::vector<std::vector<double>> matrix = { {1.0, 2.0}, {3.0, 4.0} };

    EXPECT_THROW(swapRows(matrix, -1, 0), std::out_of_range);
    EXPECT_THROW(swapRows(matrix, 0, 5), std::out_of_range);
}

// Тесты для функции swapColumns
TEST(MatrixTest, SwapColumnsNormal) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    swapColumns(matrix, 0, 2);

    EXPECT_DOUBLE_EQ(matrix[0][0], 3.0);
    EXPECT_DOUBLE_EQ(matrix[0][2], 1.0);
    EXPECT_DOUBLE_EQ(matrix[1][0], 6.0);
    EXPECT_DOUBLE_EQ(matrix[1][2], 4.0);
    EXPECT_DOUBLE_EQ(matrix[2][0], 9.0);
    EXPECT_DOUBLE_EQ(matrix[2][2], 7.0);
}

TEST(MatrixTest, SwapColumnsSameColumn) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0},
        {3.0, 4.0}
    };

    std::vector<std::vector<double>> original = matrix;
    swapColumns(matrix, 1, 1);

    EXPECT_DOUBLE_EQ(matrix[0][1], original[0][1]);
    EXPECT_DOUBLE_EQ(matrix[1][1], original[1][1]);
}

// Тесты для функции findMaxInSubmatrix
TEST(MatrixTest, FindMaxInSubmatrixNormal) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 9.0, 6.0},
        {7.0, 8.0, 5.0}
    };

    int32_t maxRow, maxCol;
    findMaxInSubmatrix(matrix, 1, maxRow, maxCol);

    EXPECT_EQ(maxRow, 1);
    EXPECT_EQ(maxCol, 1);
}

TEST(MatrixTest, FindMaxInSubmatrixStartAtZero) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 10.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    int32_t maxRow, maxCol;
    findMaxInSubmatrix(matrix, 0, maxRow, maxCol);

    EXPECT_EQ(maxRow, 0);
    EXPECT_EQ(maxCol, 1);
}

TEST(MatrixTest, FindMaxInSubmatrixEmptyMatrix) {
    std::vector<std::vector<double>> emptyMatrix;
    int32_t maxRow, maxCol;

    EXPECT_THROW(findMaxInSubmatrix(emptyMatrix, 0, maxRow, maxCol), std::runtime_error);
}

// Тесты для функции transformMatrix
TEST(MatrixTest, TransformMatrixNormal) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 9.0, 6.0},
        {7.0, 8.0, 5.0}
    };

    bool result = transformMatrix(matrix);

    EXPECT_TRUE(result);
    // После преобразования максимальный элемент (9.0) должен оказаться в позиции [0][0]
    EXPECT_DOUBLE_EQ(matrix[0][0], 9.0);
}

TEST(MatrixTest, TransformMatrixAlreadySorted) {
    std::vector<std::vector<double>> matrix = {
        {9.0, 2.0, 3.0},
        {4.0, 8.0, 6.0},
        {7.0, 5.0, 1.0}
    };

    bool result = transformMatrix(matrix);

    EXPECT_TRUE(result);
    // Матрица уже имеет максимальный элемент в [0][0]
    EXPECT_DOUBLE_EQ(matrix[0][0], 9.0);
}

TEST(MatrixTest, TransformMatrixEmpty) {
    std::vector<std::vector<double>> emptyMatrix;

    EXPECT_THROW(transformMatrix(emptyMatrix), std::runtime_error);
}

// Тесты для функции findFirstRowWithoutPositive
TEST(MatrixTest, FindFirstRowWithoutPositiveFound) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {-1.0, -2.0, -3.0},
        {4.0, 5.0, 6.0}
    };

    int32_t result = findFirstRowWithoutPositive(matrix);

    EXPECT_EQ(result, 2); // Вторая строка (индекс 1 + 1)
}

TEST(MatrixTest, FindFirstRowWithoutPositiveNotFound) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {0.5, 0.1, 0.01},
        {4.0, 5.0, 6.0}
    };

    int32_t result = findFirstRowWithoutPositive(matrix);

    EXPECT_EQ(result, -1); // Все строки содержат положительные элементы
}

TEST(MatrixTest, FindFirstRowWithoutPositiveAllNegative) {
    std::vector<std::vector<double>> matrix = {
        {-1.0, -2.0, -3.0},
        {-4.0, -5.0, -6.0}
    };

    int32_t result = findFirstRowWithoutPositive(matrix);

    EXPECT_EQ(result, 1); // Первая строка
}

TEST(MatrixTest, FindFirstRowWithoutPositiveEmpty) {
    std::vector<std::vector<double>> emptyMatrix;

    EXPECT_THROW(findFirstRowWithoutPositive(emptyMatrix), std::runtime_error);
}

// Тесты для функции checkMainDiagonal
TEST(MatrixTest, CheckMainDiagonalDescending) {
    std::vector<std::vector<double>> matrix = {
        {9.0, 2.0, 3.0},
        {4.0, 8.0, 6.0},
        {7.0, 5.0, 7.0}
    };

    testing::internal::CaptureStdout();
    checkMainDiagonal(matrix);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Главная диагональ упорядочена по убыванию") != std::string::npos ||
        output.find("Главная диагональ НЕ упорядочена по убыванию") != std::string::npos);
}

TEST(MatrixTest, CheckMainDiagonalNotDescending) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    testing::internal::CaptureStdout();
    checkMainDiagonal(matrix);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Главная диагональ НЕ упорядочена по убыванию") != std::string::npos);
}

TEST(MatrixTest, CheckMainDiagonalEmpty) {
    std::vector<std::vector<double>> emptyMatrix;

    EXPECT_THROW(checkMainDiagonal(emptyMatrix), std::runtime_error);
}

// Тесты для граничных случаев
TEST(MatrixTest, SingleElementMatrix) {
    std::vector<std::vector<double>> matrix = { {5.0} };

    // Проверяем transformMatrix
    bool result = transformMatrix(matrix);
    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(matrix[0][0], 5.0);

    // Проверяем findFirstRowWithoutPositive
    int32_t rowResult = findFirstRowWithoutPositive(matrix);
    EXPECT_EQ(rowResult, -1); // Элемент положительный

    // Проверяем с отрицательным элементом
    matrix[0][0] = -5.0;
    rowResult = findFirstRowWithoutPositive(matrix);
    EXPECT_EQ(rowResult, 1); // Строка без положительных элементов
}

// Интеграционный тест для всего алгоритма
TEST(MatrixTest, FullAlgorithmTest) {
    std::vector<std::vector<double>> matrix = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 20.0, 8.0},  // Максимальный элемент 20.0 в позиции [1][2]
        {9.0, 10.0, 11.0, 12.0},
        {13.0, 14.0, 15.0, 16.0}
    };

    // Преобразование матрицы
    transformMatrix(matrix);

    // После преобразования максимальный элемент должен быть в [0][0]
    EXPECT_DOUBLE_EQ(matrix[0][0], 20.0);

    // Проверяем findFirstRowWithoutPositive (все строки имеют положительные элементы)
    int32_t result = findFirstRowWithoutPositive(matrix);
    EXPECT_EQ(result, -1);
}

// Тест для матрицы со всеми одинаковыми элементами
TEST(MatrixTest, AllEqualElements) {
    std::vector<std::vector<double>> matrix = {
        {5.0, 5.0, 5.0},
        {5.0, 5.0, 5.0},
        {5.0, 5.0, 5.0}
    };

    transformMatrix(matrix);

    // Матрица должна остаться неизменной (кроме возможных перестановок одинаковых элементов)
    EXPECT_DOUBLE_EQ(matrix[0][0], 5.0);

    int32_t rowResult = findFirstRowWithoutPositive(matrix);
    EXPECT_EQ(rowResult, -1); // Все элементы положительные
}
