#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <cstdint>
#include "functions.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::swap;
using std::fixed;
using std::setprecision;
using std::setw;

void printMatrix(const vector<vector<double>>& matrix) {
    if (matrix.empty()) {
        cout << "Матрица пуста!" << endl;
        return;
    }

    for (const auto& row : matrix) {
        for (double val : row) {
            cout << setw(10) << fixed << setprecision(4) << val;
        }
        cout << endl;
    }
}

vector<vector<double>> createMatrix(int32_t n) {
    return vector<vector<double>>(n, vector<double>(n));
}

vector<vector<double>> inputMatrixKeyboard() {
    int32_t n;
    cout << "Введите размер квадратной матрицы: ";
    cin >> n;

    if (n <= 0) {
        throw std::invalid_argument("Размер матрицы должен быть положительным!");
    }

    auto matrix = createMatrix(n);

    cout << "Введите элементы матрицы:" << endl;
    for (size_t i = 0; i < static_cast<size_t>(n); i++) {
        for (size_t j = 0; j < static_cast<size_t>(n); j++) {
            cout << "matrix[" << i << "][" << j << "] = ";
            if (!(cin >> matrix[i][j])) {
                throw std::runtime_error("Ошибка ввода! Ожидалось число.");
            }
        }
    }
    return matrix;
}

vector<vector<double>> inputMatrixRandom() {
    int32_t n;
    double minVal, maxVal;

    cout << "Введите размер квадратной матрицы: ";
    cin >> n;

    if (n <= 0) {
        throw std::invalid_argument("Размер матрицы должен быть положительным!");
    }

    cout << "Введите минимальное значение: ";
    if (!(cin >> minVal)) {
        throw std::runtime_error("Ошибка ввода минимального значения!");
    }

    cout << "Введите максимальное значение: ";
    if (!(cin >> maxVal)) {
        throw std::runtime_error("Ошибка ввода максимального значения!");
    }

    if (minVal > maxVal) {
        throw std::invalid_argument("Минимальное значение не может быть больше максимального!");
    }

    auto matrix = createMatrix(n);
    srand(time(0));

    for (size_t i = 0; i < static_cast<size_t>(n); i++) {
        for (size_t j = 0; j < static_cast<size_t>(n); j++) {
            double randomVal = static_cast<double>(rand()) / RAND_MAX;
            matrix[i][j] = minVal + randomVal * (maxVal - minVal);
        }
    }

    return matrix;
}

vector<vector<double>> inputMatrix() {
    int32_t choice;
    cout << "Выберите способ заполнения матрицы:" << endl;
    cout << "1 - Ввод с клавиатуры" << endl;
    cout << "2 - Заполнение случайными числами" << endl;
    cout << "Ваш выбор: ";

    if (!(cin >> choice)) {
        throw std::runtime_error("Ошибка ввода выбора!");
    }

    switch (choice) {
    case 1:
        return inputMatrixKeyboard();
    case 2:
        return inputMatrixRandom();
    default:
        cout << "Неверный выбор! Используется ввод с клавиатуры." << endl;
        return inputMatrixKeyboard();
    }
}

void findMaxInSubmatrix(const vector<vector<double>>& matrix, int32_t start, int32_t& maxRow, int32_t& maxCol) {
    if (matrix.empty()) {
        throw std::runtime_error("Матрица пуста!");
    }

    if (start >= static_cast<int32_t>(matrix.size())) {
        throw std::out_of_range("Начальный индекс выходит за границы матрицы!");
    }

    double maxVal = matrix[start][start];
    maxRow = start;
    maxCol = start;

    for (size_t i = static_cast<size_t>(start); i < matrix.size(); i++) {
        for (size_t j = static_cast<size_t>(start); j < matrix[i].size(); j++) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxRow = static_cast<int32_t>(i);
                maxCol = static_cast<int32_t>(j);
            }
        }
    }
}

void swapRows(vector<vector<double>>& matrix, int32_t row1, int32_t row2) {
    if (row1 < 0 || row2 < 0 || row1 >= static_cast<int32_t>(matrix.size()) || row2 >= static_cast<int32_t>(matrix.size())) {
        throw std::out_of_range("Индекс строки выходит за границы матрицы!");
    }

    if (row1 == row2) {
        return;
    }

    for (size_t j = 0; j < matrix[static_cast<size_t>(row1)].size(); j++) {
        swap(matrix[static_cast<size_t>(row1)][j], matrix[static_cast<size_t>(row2)][j]);
    }
}

void swapColumns(vector<vector<double>>& matrix, int32_t col1, int32_t col2) {
    if (matrix.empty()) {
        throw std::runtime_error("Матрица пуста!");
    }

    if (col1 < 0 || col2 < 0 || col1 >= static_cast<int32_t>(matrix[0].size()) || col2 >= static_cast<int32_t>(matrix[0].size())) {
        throw std::out_of_range("Индекс столбца выходит за границы матрицы!");
    }

    if (col1 == col2) {
        return;
    }

    for (size_t i = 0; i < matrix.size(); i++) {
        swap(matrix[i][static_cast<size_t>(col1)], matrix[i][static_cast<size_t>(col2)]);
    }
}

bool transformMatrix(vector<vector<double>>& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error("Матрица пуста!");
    }

    int32_t n = static_cast<int32_t>(matrix.size());

    for (int32_t k = 0; k < n; k++) {
        int32_t maxRow, maxCol;
        findMaxInSubmatrix(matrix, k, maxRow, maxCol);

        if (maxRow == -1 || maxCol == -1) {
            throw std::runtime_error("Не удалось найти максимальный элемент в подматрице!");
        }

        swapRows(matrix, k, maxRow);
        swapColumns(matrix, k, maxCol);
    }
    return true;
}

int32_t findFirstRowWithoutPositive(const vector<vector<double>>& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error("Матрица пуста!");
    }

    for (size_t i = 0; i < matrix.size(); i++) {
        bool hasPositive = false;
        for (size_t j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] > 0) {
                hasPositive = true;
                break;
            }
        }
        if (!hasPositive) {
            return static_cast<int32_t>(i + 1);
        }
    }
    return -1;
}

void checkMainDiagonal(const vector<vector<double>>& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error("Матрица пуста!");
    }

    cout << "Элементы главной диагонали: ";
    for (size_t i = 0; i < matrix.size(); i++) {
        cout << matrix[i][i] << " ";
    }
    cout << endl;

    bool isDescending = true;
    for (size_t i = 1; i < matrix.size(); i++) {
        if (matrix[i][i] > matrix[i - 1][i - 1]) {
            isDescending = false;
            break;
        }
    }

    if (isDescending) {
        cout << "Главная диагональ упорядочена по убыванию." << endl;
    }
    else {
        cout << "Главная диагональ НЕ упорядочена по убыванию." << endl;
    }
}