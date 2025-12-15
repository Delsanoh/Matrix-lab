#include "Matrix_Operations.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

CMatrix::CMatrix() : data(nullptr), rows(0), cols(0) {}

CMatrix::CMatrix(std::size_t r, std::size_t c) : rows(r), cols(c) {
    if (r == 0 || c == 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }

    data = new double* [rows];

    for (std::size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        if (!data[i]) {
            for (std::size_t j = 0; j < i; ++j) {
                delete[] data[j];
                data[j] = nullptr;
            }
            delete[] data;
            data = nullptr;
        }

        for (std::size_t j = 0; j < cols; ++j) {
            data[i][j] = 0.0;
        }
    }
}

CMatrix::~CMatrix() {
    clear();
}

void CMatrix::clear() {
    if (data) {
        for (std::size_t i = 0; i < rows; ++i) {
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

    data = new double* [rows];

    for (std::size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        if (!data[i]) {
            for (std::size_t j = 0; j < i; ++j) {
                delete[] data[j];
            }
            delete[] data;
            throw std::bad_alloc();
        }

        for (std::size_t j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

CMatrix& CMatrix::operator=(const CMatrix& other) {
    if (this != &other) {
        clear();
        rows = other.rows;
        cols = other.cols;

        if (other.data == nullptr) {
            data = nullptr;
            return *this;
        }

        data = new double* [rows];
        if (!data) throw std::bad_alloc();

        for (std::size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols];
            if (!data[i]) {
                for (std::size_t j = 0; j < i; ++j) {
                    delete[] data[j];
                }
                delete[] data;
                throw std::bad_alloc();
            }

            for (std::size_t j = 0; j < cols; ++j) {
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

void CMatrix::resize(std::size_t r, std::size_t c) {
    if (r == 0 || c == 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }

    if (r == rows && c == cols) {
        return;
    }

    CMatrix newMatrix(r, c);
    std::size_t minRows = std::min(r, rows);
    std::size_t minCols = std::min(c, cols);

    for (std::size_t i = 0; i < minRows; ++i) {
        for (std::size_t j = 0; j < minCols; ++j) {
            newMatrix.data[i][j] = data[i][j];
        }
    }

    *this = std::move(newMatrix);
}

double& CMatrix::operator()(std::size_t row, std::size_t col) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

const double& CMatrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

bool CMatrix::isSquare() const {
    return rows == cols;
}

bool CMatrix::isEmpty() const {
    return rows == 0 || cols == 0 || data == nullptr;
}

std::ostream& operator<<(std::ostream& os, const CMatrix& matrix) {
    if (matrix.isEmpty()) {
        os << "Empty matrix" << std::endl;
        return os;
    }

    for (std::size_t i = 0; i < matrix.rows; ++i) {
        for (std::size_t j = 0; j < matrix.cols; ++j) {
            os << std::setw(12) << std::setprecision(4) << std::fixed << matrix.data[i][j];
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, CMatrix& matrix) {
    if (matrix.isEmpty()) {
        throw std::runtime_error("Cannot input into empty matrix");
    }

    std::cout << "Enter matrix " << matrix.rows << "x" << matrix.cols << ":" << std::endl;
    for (std::size_t i = 0; i < matrix.rows; ++i) {
        for (std::size_t j = 0; j < matrix.cols; ++j) {
            if (!(is >> matrix.data[i][j])) {
                throw std::runtime_error("Invalid input for matrix element");
            }
        }
    }
    return is;
}


// ТУТ БЫЛІ ПЕРЕГРУЗКИ И КОНСТРУКТОРЫ!!!!!!!!!!!!!!
//ТУТ БУДУТ ФУНКЦИИ ДЛЯ СИШНЫХ МАТРИЦ!!!!!!!!!!!!

CMatrix createCMatrix(std::size_t rows, std::size_t cols) {
    return CMatrix(rows, cols);
}

CMatrix createRandomCMatrix(std::size_t rows, std::size_t cols, double minVal, double maxVal) {
    CMatrix mat(rows, cols);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            double randomVal = static_cast<double>(std::rand()) / RAND_MAX;
            mat(i, j) = minVal + randomVal * (maxVal - minVal);
        }
    }

    return mat;
}

void findMaxInCSubmatrix(const CMatrix& matrix, std::size_t start, std::size_t& maxRow, std::size_t& maxCol) {
    if (matrix.isEmpty()) {
        throw std::runtime_error("Matrix is empty");
    }

    if (start >= matrix.rows || start >= matrix.cols) {
        throw std::out_of_range("Start index out of range");
    }

    double maxVal = std::numeric_limits<double>::lowest();
    maxRow = start;
    maxCol = start;

    for (std::size_t i = start; i < matrix.rows; ++i) {
        for (std::size_t j = start; j < matrix.cols; ++j) {
            if (matrix(i, j) > maxVal) {
                maxVal = matrix(i, j);
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

void swapCRows(CMatrix& matrix, std::size_t row1, std::size_t row2) {
    if (row1 == row2) return;

    if (row1 >= matrix.rows || row2 >= matrix.rows) {
        throw std::out_of_range("Row index out of range");
    }

    for (std::size_t j = 0; j < matrix.cols; ++j) {
        std::swap(matrix(row1, j), matrix(row2, j));
    }
}

void swapCColumns(CMatrix& matrix, std::size_t col1, std::size_t col2) {
    if (col1 == col2) return;

    if (col1 >= matrix.cols || col2 >= matrix.cols) {
        throw std::out_of_range("Column index out of range");
    }

    for (std::size_t i = 0; i < matrix.rows; ++i) {
        std::swap(matrix(i, col1), matrix(i, col2));
    }
}

void transformCMatrix(CMatrix& matrix) {
    if (!matrix.isSquare()) {
        throw std::runtime_error("Matrix must be square for transformation");
    }

    if (matrix.isEmpty()) {
        throw std::runtime_error("Matrix is empty");
    }

    for (std::size_t k = 0; k < matrix.rows; ++k) {
        std::size_t maxRow, maxCol;
        findMaxInCSubmatrix(matrix, k, maxRow, maxCol);

        swapCRows(matrix, k, maxRow);
        swapCColumns(matrix, k, maxCol);
    }
}

int findFirstRowWithoutPositiveC(const CMatrix& matrix) {
    if (matrix.isEmpty()) {
        return -1;
    }

    for (std::size_t i = 0; i < matrix.rows; ++i) {
        bool hasPositive = false;
        for (std::size_t j = 0; j < matrix.cols; ++j) {
            if (matrix(i, j) > 0) {
                hasPositive = true;
                break;
            }
        }
        if (!hasPositive) {
            return static_cast<int>(i + 1);
        }
    }
    return -1;
}

bool isDiagonalDescendingC(const CMatrix& matrix) {
    if (!matrix.isSquare() || matrix.isEmpty()) {
        return false;
    }

    for (std::size_t i = 1; i < matrix.rows; ++i) {
        if (matrix(i, i) > matrix(i - 1, i - 1)) {
            return false;
        }
    }
    return true;
}

void printDiagonalC(const CMatrix& matrix) {
    if (!matrix.isSquare() || matrix.isEmpty()) {
        std::cout << "Matrix is not square or empty" << std::endl;
        return;
    }

    std::cout << "Main diagonal: ";
    for (std::size_t i = 0; i < matrix.rows; ++i) {
        std::cout << matrix(i, i);
        if (i < matrix.rows - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

//Для векторов

VectorMatrix createVectorMatrix(std::size_t rows, std::size_t cols) {
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    return VectorMatrix(rows, std::vector<double>(cols, 0.0));
}

void printVectorMatrix(const VectorMatrix& matrix) {
    if (matrix.empty()) {
        std::cout << "Empty matrix" << std::endl;
        return;
    }

    for (const auto& row : matrix) {
        for (double val : row) {
            std::cout << std::setw(12) << std::setprecision(4) << std::fixed << val;
        }
        std::cout << std::endl;
    }
}

void inputVectorMatrix(VectorMatrix& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error("Matrix is empty");
    }

    std::size_t rows = matrix.size();
    std::size_t cols = matrix[0].size();

    std::cout << "Enter matrix " << rows << "x" << cols << ":" << std::endl;
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            if (!(std::cin >> matrix[i][j])) {
                throw std::runtime_error("Invalid input for matrix element");
            }
        }
    }
}

VectorMatrix createRandomVectorMatrix(std::size_t rows, std::size_t cols, double minVal, double maxVal) {
    VectorMatrix matrix = createVectorMatrix(rows, cols);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            double randomVal = static_cast<double>(std::rand()) / RAND_MAX;
            matrix[i][j] = minVal + randomVal * (maxVal - minVal);
        }
    }

    return matrix;
}

void findMaxInVectorSubmatrix(const VectorMatrix& matrix, std::size_t start, std::size_t& maxRow, std::size_t& maxCol) {
    if (matrix.empty()) {
        throw std::runtime_error("Matrix is empty");
    }

    if (start >= matrix.size() || start >= matrix[0].size()) {
        throw std::out_of_range("Start index out of range");
    }

    double maxVal = std::numeric_limits<double>::lowest();
    maxRow = start;
    maxCol = start;

    for (std::size_t i = start; i < matrix.size(); ++i) {
        for (std::size_t j = start; j < matrix[i].size(); ++j) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxRow = i;
                maxCol = j;
            }
        }
    }
}

void swapVectorRows(VectorMatrix& matrix, std::size_t row1, std::size_t row2) {
    if (row1 == row2) return;

    if (row1 >= matrix.size() || row2 >= matrix.size()) {
        throw std::out_of_range("Row index out of range");
    }

    std::swap(matrix[row1], matrix[row2]);
}

void swapVectorColumns(VectorMatrix& matrix, std::size_t col1, std::size_t col2) {
    if (col1 == col2) return;

    if (matrix.empty() || col1 >= matrix[0].size() || col2 >= matrix[0].size()) {
        throw std::out_of_range("Column index out of range");
    }

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        std::swap(matrix[i][col1], matrix[i][col2]);
    }
}

void transformVectorMatrix(VectorMatrix& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error("Matrix is empty");
    }

    if (matrix.size() != matrix[0].size()) {
        throw std::runtime_error("Matrix must be square for transformation");
    }

    for (std::size_t k = 0; k < matrix.size(); ++k) {
        std::size_t maxRow, maxCol;
        findMaxInVectorSubmatrix(matrix, k, maxRow, maxCol); //СТАРТОВАЯ ПОЗИЦИЯ ДЛЯ ПОДМАТРИЦЫ ПОЗВОЛЯЕТ
        //С КАЖДЫМ ПРОХОДОМ ИГНОРИРОВАТЬ СТРОКИ И СТОЛБЦЫ, КОТОРЫЕ МЫ УЖЕ ПРЕОБРАЗОВАЛИ

        swapVectorRows(matrix, k, maxRow);
        swapVectorColumns(matrix, k, maxCol);
    }
}

int findFirstRowWithoutPositiveVector(const VectorMatrix& matrix) {
    if (matrix.empty()) {
        return -1;
    }

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        bool hasPositive = false;
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] > 0) {
                hasPositive = true;
                break;
            }
        }
        if (!hasPositive) {
            return static_cast<int>(i + 1);
        }
    }
    return -1;
}

bool isDiagonalDescendingVector(const VectorMatrix& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        return false;
    }

    for (std::size_t i = 1; i < matrix.size(); ++i) {
        if (matrix[i][i] > matrix[i - 1][i - 1]) {
            return false;
        }
    }
    return true;
}

void printDiagonalVector(const VectorMatrix& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        std::cout << "Matrix is not square or empty" << std::endl;
        return;
    }

    std::cout << "Main diagonal: ";
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        std::cout << matrix[i][i];
        if (i < matrix.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}