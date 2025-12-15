#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <vector>
#include <cstddef>
#include <ostream>


// C-стиль матрица
struct CMatrix {
    double** data;
    std::size_t rows;
    std::size_t cols;

    // Конструкторы
    CMatrix();
    CMatrix(std::size_t r, std::size_t c);
    ~CMatrix();

    // Управление памятью
    CMatrix(const CMatrix& other);
    CMatrix& operator=(const CMatrix& other);
    CMatrix(CMatrix&& other) noexcept;
    CMatrix& operator=(CMatrix&& other) noexcept;

    // Основные методы
    void clear();
    void resize(std::size_t r, std::size_t c);

    // Доступ к элементам
    double& operator()(std::size_t row, std::size_t col);
    const double& operator()(std::size_t row, std::size_t col) const;

    // Проверки
    bool isSquare() const;
    bool isEmpty() const;

    // Дружественные функции ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const CMatrix& matrix);
    friend std::istream& operator>>(std::istream& is, CMatrix& matrix);
};

// Псевдоним для STL матрицы
using VectorMatrix = std::vector<std::vector<double>>;


// Создание и управление
CMatrix createCMatrix(std::size_t rows, std::size_t cols);
CMatrix createRandomCMatrix(std::size_t rows, std::size_t cols, double minVal, double maxVal);

// Основные операции для задачи
//закомментіть место где ігноріруетс своя строка і столбец для рассматріваемого элемента
void findMaxInCSubmatrix(const CMatrix& matrix, std::size_t start, std::size_t& maxRow, std::size_t& maxCol);
void swapCRows(CMatrix& matrix, std::size_t row1, std::size_t row2);
void swapCColumns(CMatrix& matrix, std::size_t col1, std::size_t col2);
void transformCMatrix(CMatrix& matrix);
int findFirstRowWithoutPositiveC(const CMatrix& matrix);
bool isDiagonalDescendingC(const CMatrix& matrix);
void printDiagonalC(const CMatrix& matrix);


// Создание и управление
VectorMatrix createVectorMatrix(std::size_t rows, std::size_t cols);
VectorMatrix createRandomVectorMatrix(std::size_t rows, std::size_t cols, double minVal, double maxVal);
void printVectorMatrix(const VectorMatrix& matrix);
void inputVectorMatrix(VectorMatrix& matrix);

// Основные операции для задачи
void findMaxInVectorSubmatrix(const VectorMatrix& matrix, std::size_t start, std::size_t& maxRow, std::size_t& maxCol);
void swapVectorRows(VectorMatrix& matrix, std::size_t row1, std::size_t row2);
void swapVectorColumns(VectorMatrix& matrix, std::size_t col1, std::size_t col2);
void transformVectorMatrix(VectorMatrix& matrix);
int findFirstRowWithoutPositiveVector(const VectorMatrix& matrix);
bool isDiagonalDescendingVector(const VectorMatrix& matrix);
void printDiagonalVector(const VectorMatrix& matrix);

#endif // MATRIX_OPERATIONS_H