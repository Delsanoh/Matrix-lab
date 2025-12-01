#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <locale>
#include "functions.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        auto matrix = inputMatrix();

        cout << "\nИсходная матрица:" << endl;
        printMatrix(matrix);

        transformMatrix(matrix);

        cout << "\nИтоговая преобразованная матрица:" << endl;
        printMatrix(matrix);

        int32_t result = findFirstRowWithoutPositive(matrix);

        if (result != -1) {
            cout << "\nПервая строка без положительных элементов: " << result << endl;
        }
        else {
            cout << "\nВ матрице нет строк без положительных элементов" << endl;
        }

        checkMainDiagonal(matrix);

    }
    catch (const std::exception& e) {
        cout << "\nОшибка: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cout << "\nНеизвестная ошибка!" << endl;
        return 1;
    }

    return 0;
}