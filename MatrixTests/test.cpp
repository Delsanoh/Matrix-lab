#include "pch.h"

#include "../Лаба 4/Matrix.cpp"
#include "../Лаба 4/Matrix_Operations.cpp"
#include "gtest/gtest.h"
#include <vector>
#include <stdexcept>

// ============================================
// ТЕСТЫ ДЛЯ КАСТОМНОГО КЛАССА MATRIX
// ============================================

TEST(MatrixClassTest, DefaultConstructor) {
    Matrix<double> m;
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_EQ(m.getCols(), 0);
    EXPECT_TRUE(m.isEmpty());
}

TEST(MatrixClassTest, ConstructorWithDimensions) {
    Matrix<double> m(3, 4);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 4);
    EXPECT_FALSE(m.isEmpty());

    // Проверка инициализации нулями
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            EXPECT_DOUBLE_EQ(m(i, j), 0.0);
        }
    }
}

TEST(MatrixClassTest, AccessOperators) {
    Matrix<double> m(2, 2);
    m(0, 0) = 1.0;
    m(0, 1) = 2.0;
    m(1, 0) = 3.0;
    m(1, 1) = 4.0;

    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 4.0);

    // Проверка оператора []
    EXPECT_DOUBLE_EQ(m[0][0], 1.0);
    EXPECT_DOUBLE_EQ(m[0][1], 2.0);
    EXPECT_DOUBLE_EQ(m[1][0], 3.0);
    EXPECT_DOUBLE_EQ(m[1][1], 4.0);
}

TEST(MatrixClassTest, TransformSquareMatrix) {
    // Тестовая матрица 3x3
    std::vector<std::vector<double>> data = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };

    Matrix<double> m(data);
    EXPECT_TRUE(m.isSquare());

    // Выполняем преобразование
    m.transform();

    // После преобразования максимальные элементы должны быть на диагонали
    // Проверяем, что на позиции (0,0) находится максимальный элемент всей матрицы (9.0)
    EXPECT_DOUBLE_EQ(m(0, 0), 9.0);
}

TEST(MatrixClassTest, FindFirstRowWithoutPositive) {
    // Матрица, где вторая строка (индекс 1) не имеет положительных элементов
    std::vector<std::vector<double>> data = {
        {1.0, 2.0, 3.0},
        {-1.0, -2.0, -3.0},
        {4.0, 5.0, 6.0}
    };

    Matrix<double> m(data);

    // Метод возвращает номер строки (начиная с 1)
    int result = m.findFirstRowWithoutPositive();
    EXPECT_EQ(result, 2); // Вторая строка (нумерация с 1)
}

TEST(MatrixClassTest, IsDiagonalDescending) {
    // Матрица с убывающей диагональю
    std::vector<std::vector<double>> data1 = {
        {5.0, 1.0, 1.0},
        {1.0, 4.0, 1.0},
        {1.0, 1.0, 3.0}
    };

    Matrix<double> m1(data1);
    EXPECT_TRUE(m1.isDiagonalDescending());

    // Матрица с возрастающей диагональю
    std::vector<std::vector<double>> data2 = {
        {1.0, 1.0, 1.0},
        {1.0, 2.0, 1.0},
        {1.0, 1.0, 3.0}
    };

    Matrix<double> m2(data2);
    EXPECT_FALSE(m2.isDiagonalDescending());
}

TEST(MatrixClassTest, InvalidAccessThrows) {
    Matrix<double> m(2, 2);

    // Попытка доступа к несуществующему элементу должна бросать исключение
    EXPECT_THROW(m(2, 0), std::out_of_range);
    EXPECT_THROW(m(0, 2), std::out_of_range);
    EXPECT_THROW(m(2, 2), std::out_of_range);

    EXPECT_THROW(m[2], std::out_of_range);
}

// ============================================
// ТЕСТЫ ДЛЯ C-СТИЛЯ МАТРИЦ
// ============================================

TEST(CMatrixTest, ConstructorAndDestructor) {
    CMatrix m(2, 3);
    EXPECT_EQ(m.rows, 2);
    EXPECT_EQ(m.cols, 3);
    EXPECT_FALSE(m.isEmpty());

    // Проверка инициализации нулями
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(m(i, j), 0.0);
        }
    }
}

TEST(CMatrixTest, CopyConstructor) {
    CMatrix m1(2, 2);
    m1(0, 0) = 1.0;
    m1(0, 1) = 2.0;
    m1(1, 0) = 3.0;
    m1(1, 1) = 4.0;

    // Копирование
    CMatrix m2 = m1;

    EXPECT_EQ(m2.rows, 2);
    EXPECT_EQ(m2.cols, 2);
    EXPECT_DOUBLE_EQ(m2(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m2(1, 1), 4.0);

    // Изменение оригинала не должно влиять на копию
    m1(0, 0) = 99.0;
    EXPECT_DOUBLE_EQ(m2(0, 0), 1.0); // Копия не изменилась
}

TEST(CMatrixTest, TransformFunction) {
    CMatrix m(3, 3);

    // Заполняем матрицу
    m(0, 0) = 1.0; m(0, 1) = 2.0; m(0, 2) = 3.0;
    m(1, 0) = 4.0; m(1, 1) = 9.0; m(1, 2) = 6.0; // 9.0 - максимальный элемент
    m(2, 0) = 7.0; m(2, 1) = 8.0; m(2, 2) = 5.0;

    transformCMatrix(m);

    // После преобразования максимальный элемент (9.0) должен быть на позиции (0,0)
    EXPECT_DOUBLE_EQ(m(0, 0), 9.0);
}

TEST(CMatrixTest, FindFirstRowWithoutPositiveFunction) {
    CMatrix m(3, 3);

    // Первая строка с отрицательными элементами
    m(0, 0) = -1.0; m(0, 1) = -2.0; m(0, 2) = -3.0;
    m(1, 0) = 4.0;  m(1, 1) = 5.0;  m(1, 2) = 6.0;
    m(2, 0) = 7.0;  m(2, 1) = 8.0;  m(2, 2) = 9.0;

    int result = findFirstRowWithoutPositiveC(m);
    EXPECT_EQ(result, 1); // Первая строка (нумерация с 1)
}

// ============================================
// ТЕСТЫ ДЛЯ VECTOR_STYLE МАТРИЦ
// ============================================

TEST(VectorMatrixTest, CreateAndAccess) {
    VectorMatrix m = createVectorMatrix(2, 3);
    EXPECT_EQ(m.size(), 2);
    EXPECT_EQ(m[0].size(), 3);

    // Заполняем и проверяем
    m[0][0] = 1.0; m[0][1] = 2.0; m[0][2] = 3.0;
    m[1][0] = 4.0; m[1][1] = 5.0; m[1][2] = 6.0;

    EXPECT_DOUBLE_EQ(m[0][0], 1.0);
    EXPECT_DOUBLE_EQ(m[1][2], 6.0);
}

TEST(VectorMatrixTest, TransformFunction) {
    VectorMatrix m = createVectorMatrix(3, 3);

    m[0][0] = 1.0; m[0][1] = 2.0; m[0][2] = 3.0;
    m[1][0] = 4.0; m[1][1] = 8.0; m[1][2] = 6.0; // 8.0 - максимальный
    m[2][0] = 7.0; m[2][1] = 5.0; m[2][2] = 9.0; // 9.0 - еще больше

    transformVectorMatrix(m);

    // После преобразования максимальный элемент (9.0) должен быть на позиции (0,0)
    EXPECT_DOUBLE_EQ(m[0][0], 9.0);
}

TEST(VectorMatrixTest, IsDiagonalDescendingFunction) {
    // Матрица с убывающей диагональю
    VectorMatrix m1 = {
        {5.0, 0.0, 0.0},
        {0.0, 3.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    EXPECT_TRUE(isDiagonalDescendingVector(m1));

    // Матрица не квадратная
    VectorMatrix m2 = {
        {5.0, 0.0},
        {0.0, 3.0},
        {0.0, 0.0}
    };

    EXPECT_FALSE(isDiagonalDescendingVector(m2));
}

// ============================================
// ТЕСТЫ ДЛЯ ИСКЛЮЧЕНИЙ
// ============================================

TEST(ExceptionTest, InvalidMatrixSize) {
    // Нулевые размеры должны бросать исключение
    EXPECT_THROW(Matrix<double> m(0, 5), std::invalid_argument);
    EXPECT_THROW(Matrix<double> m(5, 0), std::invalid_argument);
    EXPECT_THROW(Matrix<double> m(0, 0), std::invalid_argument);

    EXPECT_THROW(CMatrix m(0, 5), std::invalid_argument);
    EXPECT_THROW(createVectorMatrix(0, 5), std::invalid_argument);
}

TEST(ExceptionTest, InvalidIndices) {
    Matrix<double> m(2, 2);

    // Проверка различных невалидных индексов
    EXPECT_THROW(m(2, 0), std::out_of_range);
    EXPECT_THROW(m(0, 2), std::out_of_range);
    EXPECT_THROW(m(2, 2), std::out_of_range);
    EXPECT_THROW(m(100, 100), std::out_of_range);
}

// ============================================
// ТЕСТЫ ДЛЯ СЛУЧАЙНЫХ МАТРИЦ
// ============================================

TEST(RandomMatrixTest, CreateRandomMatrixClass) {
    Matrix<double> m = Matrix<double>::createRandom(3, 3, 1.0, 10.0);

    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);

    // Проверяем, что все элементы в заданном диапазоне
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_GE(m(i, j), 1.0);
            EXPECT_LE(m(i, j), 10.0);
        }
    }
}

TEST(RandomMatrixTest, CreateRandomCMatrix) {
    CMatrix m = createRandomCMatrix(2, 2, 0.0, 1.0);

    EXPECT_EQ(m.rows, 2);
    EXPECT_EQ(m.cols, 2);

    // Проверяем диапазон
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            EXPECT_GE(m(i, j), 0.0);
            EXPECT_LE(m(i, j), 1.0);
        }
    }
}
