#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "Matrix.h"
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// ========== КОНСТРУКТОРЫ ==========

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c) : rows(r), cols(c) {
    if (r == 0 || c == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }
    data.resize(rows, vector<T>(cols, T()));
}

template<typename T>
Matrix<T>::Matrix(size_t r, size_t c, T value) : rows(r), cols(c) {
    if (r == 0 || c == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }
    data.resize(rows, vector<T>(cols, value));
}

template<typename T>
Matrix<T>::Matrix(const vector<vector<T>>& input) {
    if (input.empty()) {
        rows = 0;
        cols = 0;
        return;
    }

    rows = input.size();
    cols = input[0].size();

    for (size_t i = 1; i < rows; ++i) {
        if (input[i].size() != cols) {
            throw invalid_argument("All rows must have the same length");
        }
    }

    data = input;
}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}

template<typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept
    : data(move(other.data)), rows(other.rows), cols(other.cols) {
    other.rows = 0;
    other.cols = 0;
}

// ========== ОПЕРАТОРЫ ПРИСВАИВАНИЯ ==========

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
    if (this != &other) {
        data = other.data;
        rows = other.rows;
        cols = other.cols;
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        data = move(other.data);
        rows = other.rows;
        cols = other.cols;
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

// ========== ПРОВЕРКИ ИНДЕКСОВ ==========

template<typename T>
void Matrix<T>::checkRowIndex(size_t row) const {
    if (row >= rows) {
        throw out_of_range("Row index out of range");
    }
}

template<typename T>
void Matrix<T>::checkColIndex(size_t col) const {
    if (col >= cols) {
        throw out_of_range("Column index out of range");
    }
}

template<typename T>
void Matrix<T>::checkIndices(size_t row, size_t col) const {
    checkRowIndex(row);
    checkColIndex(col);
}

// ========== ДОСТУП К ЭЛЕМЕНТАМ ==========

template<typename T>
T& Matrix<T>::operator()(size_t row, size_t col) {
    checkIndices(row, col);
    return data[row][col];
}

template<typename T>
const T& Matrix<T>::operator()(size_t row, size_t col) const {
    checkIndices(row, col);
    return data[row][col];
}

template<typename T>
vector<T>& Matrix<T>::operator[](size_t row) {
    checkRowIndex(row);
    return data[row];
}

template<typename T>
const vector<T>& Matrix<T>::operator[](size_t row) const {
    checkRowIndex(row);
    return data[row];
}

// ========== ОПЕРАЦИИ ДЛЯ ЗАДАЧИ ==========

template<typename T>
void Matrix<T>::findMaxInSubmatrix(size_t start, size_t& maxRow, size_t& maxCol) const {
    if (isEmpty()) {
        throw runtime_error("Matrix is empty");
    }

    if (start >= rows || start >= cols) {
        throw out_of_range("Start index out of range");
    }

    T maxVal = numeric_limits<T>::lowest();
    maxRow = start;
    maxCol = start;

    for (size_t i = start; i < rows; ++i) {
        for (size_t j = start; j < cols; ++j) {
            if (data[i][j] > maxVal) {
                maxVal = data[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

template<typename T>
void Matrix<T>::swapRows(size_t row1, size_t row2) {
    if (row1 == row2) {
        return;
    }

    checkRowIndex(row1);
    checkRowIndex(row2);

    swap(data[row1], data[row2]);
}

template<typename T>
void Matrix<T>::swapColumns(size_t col1, size_t col2) {
    if (col1 == col2) {
        return;
    }

    checkColIndex(col1);
    checkColIndex(col2);

    for (size_t i = 0; i < rows; ++i) {
        swap(data[i][col1], data[i][col2]);
    }
}

template<typename T>
void Matrix<T>::transform() {
    if (!isSquare()) {
        throw runtime_error("Matrix must be square for transformation");
    }

    if (isEmpty()) {
        throw runtime_error("Matrix is empty");
    }

    for (size_t k = 0; k < rows; ++k) {
        size_t maxRow, maxCol;
        findMaxInSubmatrix(k, maxRow, maxCol);

        swapRows(k, maxRow);
        swapColumns(k, maxCol);
    }
}

template<typename T>
int Matrix<T>::findFirstRowWithoutPositive() const {
    if (isEmpty()) {
        return -1;
    }

    for (size_t i = 0; i < rows; ++i) {
        bool hasPositive = false;
        for (size_t j = 0; j < cols; ++j) {
            if (data[i][j] > 0) {
                hasPositive = true;
                break;
            }
        }
        if (!hasPositive) {
            return static_cast<int>(i + 1); // +1 для пользовательской нумерации
        }
    }
    return -1;
}

template<typename T>
bool Matrix<T>::isDiagonalDescending() const {
    if (!isSquare() || isEmpty()) {
        return false;
    }

    for (size_t i = 1; i < rows; ++i) {
        if (data[i][i] > data[i - 1][i - 1]) {
            return false;
        }
    }
    return true;
}

template<typename T>
void Matrix<T>::printDiagonal() const {
    if (!isSquare() || isEmpty()) {
        cout << "Matrix is not square or empty" << endl;
        return;
    }

    cout << "Main diagonal: ";
    for (size_t i = 0; i < rows; ++i) {
        cout << data[i][i];
        if (i < rows - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

// ========== СТАТИЧЕСКИЕ МЕТОДЫ ==========

template<typename T>
Matrix<T> Matrix<T>::createRandom(size_t rows, size_t cols, T minVal, T maxVal) {
    if (rows == 0 || cols == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }

    if (minVal > maxVal) {
        throw invalid_argument("Minimum value cannot be greater than maximum value");
    }

    Matrix<T> mat(rows, cols);
    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double randomVal = static_cast<double>(rand()) / RAND_MAX;
            mat.data[i][j] = minVal + randomVal * (maxVal - minVal);
        }
    }

    return mat;
}

// Явная инстанциация шаблона для типа double
template class Matrix<double>;

// Явная инстанциация шаблонных функций для типа double
template std::ostream& operator<<(std::ostream& os, const Matrix<double>& matrix);
template std::istream& operator>>(std::istream& is, Matrix<double>& matrix);

#endif // MATRIX_CPP