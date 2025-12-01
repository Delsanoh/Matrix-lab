#include "Matrix_Operations.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

using namespace std;

// ============================================
// РЕАЛИЗАЦИЯ CMatrix
// ============================================

CMatrix::CMatrix() : data(nullptr), rows(0), cols(0) {}

CMatrix::CMatrix(size_t r, size_t c) : rows(r), cols(c) {
    if (r == 0 || c == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }

    // Выделение памяти для строк
    data = new double* [rows];
    if (!data) throw bad_alloc();

    // Выделение памяти для столбцов
    for (size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        if (!data[i]) {
            // Освобождаем уже выделенную память
            for (size_t j = 0; j < i; ++j) {
                delete[] data[j];
            }
            delete[] data;
            throw bad_alloc();
        }

        // Инициализация нулями
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = 0.0;
        }
    }
}

CMatrix::~CMatrix() {
    clear();
}

void CMatrix::clear() {
    if (data) {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
    rows = 0;
    cols = 0;
}

CMatrix::CMatrix(const CMatrix& other) : rows(other.rows), cols(other.cols) {
    if (other.data == nullptr) {
        data = nullptr;
        return;
    }

    // Выделение памяти для строк
    data = new double* [rows];
    if (!data) throw bad_alloc();

    // Выделение памяти для столбцов
    for (size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        if (!data[i]) {
            // Освобождаем уже выделенную память
            for (size_t j = 0; j < i; ++j) {
                delete[] data[j];
            }
            delete[] data;
            throw bad_alloc();
        }

        // Копирование данных
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

CMatrix& CMatrix::operator=(const CMatrix& other) {
    if (this != &other) {
        // Освобождаем текущую память
        clear();

        rows = other.rows;
        cols = other.cols;

        if (other.data == nullptr) {
            data = nullptr;
            return *this;
        }

        // Выделение памяти для строк
        data = new double* [rows];
        if (!data) throw bad_alloc();

        // Выделение памяти для столбцов
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols];
            if (!data[i]) {
                // Освобождаем уже выделенную память
                for (size_t j = 0; j < i; ++j) {
                    delete[] data[j];
                }
                delete[] data;
                throw bad_alloc();
            }

            // Копирование данных
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

CMatrix::CMatrix(CMatrix&& other) noexcept
    : data(other.data), rows(other.rows), cols(other.cols) {
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

CMatrix& CMatrix::operator=(CMatrix&& other) noexcept {
    if (this != &other) {
        clear();

        data = other.data;
        rows = other.rows;
        cols = other.cols;

        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

void CMatrix::resize(size_t r, size_t c) {
    if (r == 0 || c == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }

    // Если размеры совпадают, ничего не делаем
    if (r == rows && c == cols) {
        return;
    }

    // Создаем новую матрицу
    CMatrix newMatrix(r, c);

    // Копируем данные из старой матрицы (сколько поместится)
    size_t minRows = min(r, rows);
    size_t minCols = min(c, cols);

    for (size_t i = 0; i < minRows; ++i) {
        for (size_t j = 0; j < minCols; ++j) {
            newMatrix.data[i][j] = data[i][j];
        }
    }

    // Заменяем текущую матрицу на новую
    *this = move(newMatrix);
}

double& CMatrix::operator()(size_t row, size_t col) {
    if (row >= rows || col >= cols) {
        throw out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

const double& CMatrix::operator()(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

bool CMatrix::isSquare() const {
    return rows == cols;
}

bool CMatrix::isEmpty() const {
    return rows == 0 || cols == 0 || data == nullptr;
}

ostream& operator<<(ostream& os, const CMatrix& matrix) {
    if (matrix.isEmpty()) {
        os << "Empty matrix" << endl;
        return os;
    }

    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            os << setw(12) << setprecision(4) << fixed << matrix.data[i][j];
        }
        os << endl;
    }
    return os;
}

istream& operator>>(istream& is, CMatrix& matrix) {
    if (matrix.isEmpty()) {
        throw runtime_error("Cannot input into empty matrix");
    }

    cout << "Enter matrix " << matrix.rows << "x" << matrix.cols << ":" << endl;
    for (size_t i = 0; i < matrix.rows; ++i) {
        for (size_t j = 0; j < matrix.cols; ++j) {
            if (!(is >> matrix.data[i][j])) {
                throw runtime_error("Invalid input for matrix element");
            }
        }
    }
    return is;
}

// ============================================
// ФУНКЦИИ ДЛЯ C-СТИЛЯ МАТРИЦ
// ============================================

CMatrix createCMatrix(size_t rows, size_t cols) {
    return CMatrix(rows, cols);
}

CMatrix createRandomCMatrix(size_t rows, size_t cols, double minVal, double maxVal) {
    CMatrix mat(rows, cols);
    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double randomVal = static_cast<double>(rand()) / RAND_MAX;
            mat(i, j) = minVal + randomVal * (maxVal - minVal);
        }
    }

    return mat;
}

void findMaxInCSubmatrix(const CMatrix& matrix, size_t start, size_t& maxRow, size_t& maxCol) {
    if (matrix.isEmpty()) {
        throw runtime_error("Matrix is empty");
    }

    if (start >= matrix.rows || start >= matrix.cols) {
        throw out_of_range("Start index out of range");
    }

    double maxVal = numeric_limits<double>::lowest();
    maxRow = start;
    maxCol = start;

    for (size_t i = start; i < matrix.rows; ++i) {
        for (size_t j = start; j < matrix.cols; ++j) {
            if (matrix(i, j) > maxVal) {
                maxVal = matrix(i, j);
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

void swapCRows(CMatrix& matrix, size_t row1, size_t row2) {
    if (row1 == row2) return;

    if (row1 >= matrix.rows || row2 >= matrix.rows) {
        throw out_of_range("Row index out of range");
    }

    for (size_t j = 0; j < matrix.cols; ++j) {
        swap(matrix(row1, j), matrix(row2, j));
    }
}

void swapCColumns(CMatrix& matrix, size_t col1, size_t col2) {
    if (col1 == col2) return;

    if (col1 >= matrix.cols || col2 >= matrix.cols) {
        throw out_of_range("Column index out of range");
    }

    for (size_t i = 0; i < matrix.rows; ++i) {
        swap(matrix(i, col1), matrix(i, col2));
    }
}

void transformCMatrix(CMatrix& matrix) {
    if (!matrix.isSquare()) {
        throw runtime_error("Matrix must be square for transformation");
    }

    if (matrix.isEmpty()) {
        throw runtime_error("Matrix is empty");
    }

    for (size_t k = 0; k < matrix.rows; ++k) {
        size_t maxRow, maxCol;
        findMaxInCSubmatrix(matrix, k, maxRow, maxCol);

        swapCRows(matrix, k, maxRow);
        swapCColumns(matrix, k, maxCol);
    }
}

int findFirstRowWithoutPositiveC(const CMatrix& matrix) {
    if (matrix.isEmpty()) {
        return -1;
    }

    for (size_t i = 0; i < matrix.rows; ++i) {
        bool hasPositive = false;
        for (size_t j = 0; j < matrix.cols; ++j) {
            if (matrix(i, j) > 0) {
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

bool isDiagonalDescendingC(const CMatrix& matrix) {
    if (!matrix.isSquare() || matrix.isEmpty()) {
        return false;
    }

    for (size_t i = 1; i < matrix.rows; ++i) {
        if (matrix(i, i) > matrix(i - 1, i - 1)) {
            return false;
        }
    }
    return true;
}

void printDiagonalC(const CMatrix& matrix) {
    if (!matrix.isSquare() || matrix.isEmpty()) {
        cout << "Matrix is not square or empty" << endl;
        return;
    }

    cout << "Main diagonal: ";
    for (size_t i = 0; i < matrix.rows; ++i) {
        cout << matrix(i, i);
        if (i < matrix.rows - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

// ============================================
// ФУНКЦИИ ДЛЯ VECTOR_STYLE МАТРИЦ
// ============================================

VectorMatrix createVectorMatrix(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        throw invalid_argument("Matrix dimensions must be positive");
    }
    return VectorMatrix(rows, vector<double>(cols, 0.0));
}

void printVectorMatrix(const VectorMatrix& matrix) {
    if (matrix.empty()) {
        cout << "Empty matrix" << endl;
        return;
    }

    for (const auto& row : matrix) {
        for (double val : row) {
            cout << setw(12) << setprecision(4) << fixed << val;
        }
        cout << endl;
    }
}

void inputVectorMatrix(VectorMatrix& matrix) {
    if (matrix.empty()) {
        throw runtime_error("Matrix is empty");
    }

    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    cout << "Enter matrix " << rows << "x" << cols << ":" << endl;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (!(cin >> matrix[i][j])) {
                throw runtime_error("Invalid input for matrix element");
            }
        }
    }
}

VectorMatrix createRandomVectorMatrix(size_t rows, size_t cols, double minVal, double maxVal) {
    VectorMatrix matrix = createVectorMatrix(rows, cols);
    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            double randomVal = static_cast<double>(rand()) / RAND_MAX;
            matrix[i][j] = minVal + randomVal * (maxVal - minVal);
        }
    }

    return matrix;
}

void findMaxInVectorSubmatrix(const VectorMatrix& matrix, size_t start, size_t& maxRow, size_t& maxCol) {
    if (matrix.empty()) {
        throw runtime_error("Matrix is empty");
    }

    if (start >= matrix.size() || start >= matrix[0].size()) {
        throw out_of_range("Start index out of range");
    }

    double maxVal = numeric_limits<double>::lowest();
    maxRow = start;
    maxCol = start;

    for (size_t i = start; i < matrix.size(); ++i) {
        for (size_t j = start; j < matrix[i].size(); ++j) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

void swapVectorRows(VectorMatrix& matrix, size_t row1, size_t row2) {
    if (row1 == row2) return;

    if (row1 >= matrix.size() || row2 >= matrix.size()) {
        throw out_of_range("Row index out of range");
    }

    swap(matrix[row1], matrix[row2]);
}

void swapVectorColumns(VectorMatrix& matrix, size_t col1, size_t col2) {
    if (col1 == col2) return;

    if (matrix.empty() || col1 >= matrix[0].size() || col2 >= matrix[0].size()) {
        throw out_of_range("Column index out of range");
    }

    for (size_t i = 0; i < matrix.size(); ++i) {
        swap(matrix[i][col1], matrix[i][col2]);
    }
}

void transformVectorMatrix(VectorMatrix& matrix) {
    if (matrix.empty()) {
        throw runtime_error("Matrix is empty");
    }

    if (matrix.size() != matrix[0].size()) {
        throw runtime_error("Matrix must be square for transformation");
    }

    for (size_t k = 0; k < matrix.size(); ++k) {
        size_t maxRow, maxCol;
        findMaxInVectorSubmatrix(matrix, k, maxRow, maxCol);

        swapVectorRows(matrix, k, maxRow);
        swapVectorColumns(matrix, k, maxCol);
    }
}

int findFirstRowWithoutPositiveVector(const VectorMatrix& matrix) {
    if (matrix.empty()) {
        return -1;
    }

    for (size_t i = 0; i < matrix.size(); ++i) {
        bool hasPositive = false;
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] > 0) {
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

bool isDiagonalDescendingVector(const VectorMatrix& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        return false;
    }

    for (size_t i = 1; i < matrix.size(); ++i) {
        if (matrix[i][i] > matrix[i - 1][i - 1]) {
            return false;
        }
    }
    return true;
}

void printDiagonalVector(const VectorMatrix& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        cout << "Matrix is not square or empty" << endl;
        return;
    }

    cout << "Main diagonal: ";
    for (size_t i = 0; i < matrix.size(); ++i) {
        cout << matrix[i][i];
        if (i < matrix.size() - 1) {
            cout << " ";
        }
    }
    cout << endl;
}