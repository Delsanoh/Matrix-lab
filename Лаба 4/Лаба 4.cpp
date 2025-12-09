#include <iostream>
#include <locale>
#include "Matrix.h"
#include "Matrix_Operations.h"

enum class MatrixType {
    C_STYLE = 1,
    VECTOR_STYLE = 2,
    MATRIX_CLASS = 3
};

MatrixType chooseMatrixType() {
    int choice;
    std::cout << "\nВыберите тип матрицы:" << std::endl;
    std::cout << "1 - C-стиль (сырые указатели)" << std::endl;
    std::cout << "2 - STL стиль (vector<vector<double>>)" << std::endl;
    std::cout << "3 - Кастомный класс Matrix" << std::endl;
    std::cout << "Ваш выбор: ";

    if (!(std::cin >> choice)) {
        throw std::runtime_error("Ошибка ввода выбора типа матрицы");
    }

    switch (choice) {
    case 1: return MatrixType::C_STYLE;
    case 2: return MatrixType::VECTOR_STYLE;
    case 3: return MatrixType::MATRIX_CLASS;
    default:
        std::cout << "Неверный выбор! Используется кастомный класс Matrix." << std::endl;
        return MatrixType::MATRIX_CLASS;
    }
}

int chooseInputMethod() {
    int choice;
    std::cout << "\nВыберите способ создания матрицы:" << std::endl;
    std::cout << "1 - Ввод с клавиатуры" << std::endl;
    std::cout << "2 - Случайное заполнение" << std::endl;
    std::cout << "Ваш выбор: ";

    if (!(std::cin >> choice)) {
        throw std::runtime_error("Ошибка ввода выбора способа ввода");
    }

    return choice;
}

void getMatrixSize(std::size_t& rows, std::size_t& cols) {
    std::cout << "Введите количество строк: ";
    std::cin >> rows;
    std::cout << "Введите количество столбцов: ";
    std::cin >> cols;

    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Размеры матрицы должны быть положительными");
    }
}

void getRandomRange(double& minVal, double& maxVal) {
    std::cout << "Введите минимальное значение: ";
    std::cin >> minVal;
    std::cout << "Введите максимальное значение: ";
    std::cin >> maxVal;

    if (minVal > maxVal) {
        throw std::invalid_argument("Минимальное значение не может быть больше максимального");
    }
}

void demonstrateCStyle() {
    std::cout << "\n=== РАБОТА С C-СТИЛЕМ (СЫРЫЕ УКАЗАТЕЛИ) ===" << std::endl;

    try {
        std::size_t rows, cols;
        getMatrixSize(rows, cols);

        CMatrix matrix(rows, cols);

        int inputMethod = chooseInputMethod();
        if (inputMethod == 1) {
            std::cin >> matrix;
        }
        else {
            double minVal, maxVal;
            getRandomRange(minVal, maxVal);
            matrix = createRandomCMatrix(rows, cols, minVal, maxVal);
        }

        std::cout << "\nИсходная матрица:" << std::endl;
        std::cout << matrix;

        if (matrix.isSquare()) {
            std::cout << "\nВыполняем преобразование матрицы..." << std::endl;
            transformCMatrix(matrix);
            std::cout << "Преобразованная матрица:" << std::endl;
            std::cout << matrix;
        }
        else {
            std::cout << "\nМатрица не квадратная, преобразование невозможно" << std::endl;
        }

        int rowWithoutPos = findFirstRowWithoutPositiveC(matrix);
        if (rowWithoutPos != -1) {
            std::cout << "\nПервая строка без положительных элементов: " << rowWithoutPos << std::endl;
        }
        else {
            std::cout << "\nВсе строки содержат положительные элементы" << std::endl;
        }

        if (matrix.isSquare()) {
            printDiagonalC(matrix);
            if (isDiagonalDescendingC(matrix)) {
                std::cout << "Главная диагональ упорядочена по убыванию" << std::endl;
            }
            else {
                std::cout << "Главная диагональ НЕ упорядочена по убыванию" << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\nОшибка: " << e.what() << std::endl;
        throw;
    }
}

void demonstrateVectorStyle() {
    std::cout << "\n=== РАБОТА С STL СТИЛЕМ (vector<vector<double>>) ===" << std::endl;

    try {
        std::size_t rows, cols;
        getMatrixSize(rows, cols);

        VectorMatrix matrix = createVectorMatrix(rows, cols);

        int inputMethod = chooseInputMethod();
        if (inputMethod == 1) {
            inputVectorMatrix(matrix);
        }
        else {
            double minVal, maxVal;
            getRandomRange(minVal, maxVal);
            matrix = createRandomVectorMatrix(rows, cols, minVal, maxVal);
        }

        std::cout << "\nИсходная матрица:" << std::endl;
        printVectorMatrix(matrix);

        if (matrix.size() == matrix[0].size()) {
            std::cout << "\nВыполняем преобразование матрицы..." << std::endl;
            transformVectorMatrix(matrix);
            std::cout << "Преобразованная матрица:" << std::endl;
            printVectorMatrix(matrix);
        }
        else {
            std::cout << "\nМатрица не квадратная, преобразование невозможно" << std::endl;
        }

        int rowWithoutPos = findFirstRowWithoutPositiveVector(matrix);
        if (rowWithoutPos != -1) {
            std::cout << "\nПервая строка без положительных элементов: " << rowWithoutPos << std::endl;
        }
        else {
            std::cout << "\nВсе строки содержат положительные элементы" << std::endl;
        }

        if (matrix.size() == matrix[0].size()) {
            printDiagonalVector(matrix);
            if (isDiagonalDescendingVector(matrix)) {
                std::cout << "Главная диагональ упорядочена по убыванию" << std::endl;
            }
            else {
                std::cout << "Главная диагональ НЕ упорядочена по убыванию" << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\nОшибка: " << e.what() << std::endl;
        throw;
    }
}

void demonstrateMatrixClass() {
    std::cout << "\n=== РАБОТА С КАСТОМНЫМ КЛАССОМ MATRIX ===" << std::endl;

    try {
        std::size_t rows, cols;
        getMatrixSize(rows, cols);

        Matrix<double> matrix(rows, cols);

        int inputMethod = chooseInputMethod();
        if (inputMethod == 1) {
            std::cin >> matrix;
        }
        else {
            double minVal, maxVal;
            getRandomRange(minVal, maxVal);
            matrix = Matrix<double>::createRandom(rows, cols, minVal, maxVal);
        }

        std::cout << "\nИсходная матрица:" << std::endl;
        std::cout << matrix;

        if (matrix.isSquare()) {
            std::cout << "\nВыполняем преобразование матрицы..." << std::endl;
            matrix.transform();
            std::cout << "Преобразованная матрица:" << std::endl;
            std::cout << matrix;
        }
        else {
            std::cout << "\nМатрица не квадратная, преобразование невозможно" << std::endl;
        }

        int rowWithoutPos = matrix.findFirstRowWithoutPositive();
        if (rowWithoutPos != -1) {
            std::cout << "\nПервая строка без положительных элементов: " << rowWithoutPos << std::endl;
        }
        else {
            std::cout << "\nВсе строки содержат положительные элементы" << std::endl;
        }

        if (matrix.isSquare()) {
            matrix.printDiagonal();
            if (matrix.isDiagonalDescending()) {
                std::cout << "Главная диагональ упорядочена по убыванию" << std::endl;
            }
            else {
                std::cout << "Главная диагональ НЕ упорядочена по убыванию" << std::endl;
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\nОшибка: " << e.what() << std::endl;
        throw;
    }
}

int main() {
    std::locale::global(std::locale(""));

    std::cout << "ПРОГРАММА ДЛЯ РАБОТЫ С МАТРИЦАМИ" << std::endl;

    try {
        MatrixType matrixType = chooseMatrixType();

        switch (matrixType) {
        case MatrixType::C_STYLE:
            demonstrateCStyle();
            break;
        case MatrixType::VECTOR_STYLE:
            demonstrateVectorStyle();
            break;
        case MatrixType::MATRIX_CLASS:
            demonstrateMatrixClass();
            break;
        default:
            std::cout << "Неизвестный тип матрицы!" << std::endl;
            return 1;
        }

        std::cout << "\nПрограмма успешно завершена!" << std::endl;

    }
    catch (const std::bad_alloc& e) {
        std::cerr << "\nКритическая ошибка: Недостаточно памяти!" << std::endl;
        return 2;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "\nОшибка аргумента: " << e.what() << std::endl;
        return 3;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "\nОшибка диапазона: " << e.what() << std::endl;
        return 4;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "\nОшибка выполнения: " << e.what() << std::endl;
        return 5;
    }
    catch (const std::exception& e) {
        std::cerr << "\nСтандартная ошибка: " << e.what() << std::endl;
        return 6;
    }
    catch (...) {
        std::cerr << "\nНеизвестная ошибка!" << std::endl;
        return 7;
    }

    return 0;
}