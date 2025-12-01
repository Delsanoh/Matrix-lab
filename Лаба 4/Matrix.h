#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

template<typename T = double>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    // ========== КОНСТРУКТОРЫ ==========
    Matrix() : rows(0), cols(0) {}
    Matrix(size_t r, size_t c);
    Matrix(size_t r, size_t c, T value);
    Matrix(const std::vector<std::vector<T>>& input);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;

    // ========== ОПЕРАТОРЫ ПРИСВАИВАНИЯ ==========
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    // ========== ДОСТУП К ЭЛЕМЕНТАМ ==========
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    bool isEmpty() const { return rows == 0 || cols == 0; }
    bool isSquare() const { return rows == cols; }

    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;

    std::vector<T>& operator[](size_t row);
    const std::vector<T>& operator[](size_t row) const;

    // ========== ОПЕРАЦИИ ДЛЯ ЗАДАЧИ ==========
    void transform();
    int findFirstRowWithoutPositive() const;
    bool isDiagonalDescending() const;
    void printDiagonal() const;

    // ========== ВВОД/ВЫВОД ==========
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);

    template<typename U>
    friend std::istream& operator>>(std::istream& is, Matrix<U>& matrix);

    // ========== СТАТИЧЕСКИЕ МЕТОДЫ ==========
    static Matrix createRandom(size_t rows, size_t cols, T minVal, T maxVal);

private:
    void checkRowIndex(size_t row) const;
    void checkColIndex(size_t col) const;
    void checkIndices(size_t row, size_t col) const;
    void findMaxInSubmatrix(size_t start, size_t& maxRow, size_t& maxCol) const;
    void swapRows(size_t row1, size_t row2);
    void swapColumns(size_t col1, size_t col2);
};

// ========== ВНЕШНИЕ ШАБЛОННЫЕ ФУНКЦИИ ==========

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    if (matrix.isEmpty()) {
        os << "Empty matrix" << std::endl;
        return os;
    }

    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << std::setw(12) << std::setprecision(4) << std::fixed << matrix.data[i][j];
        }
        os << std::endl;
    }
    return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& matrix) {
    if (matrix.isEmpty()) {
        throw std::runtime_error("Cannot input into empty matrix");
    }

    std::cout << "Enter matrix " << matrix.rows << "x" << matrix.cols << ":" << std::endl;
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            if (!(is >> matrix.data[i][j])) {
                throw std::runtime_error("Invalid input for matrix element");
            }
        }
    }
    return is;
}

// Включение реализации шаблонного класса
#include "Matrix.cpp"

#endif // MATRIX_H