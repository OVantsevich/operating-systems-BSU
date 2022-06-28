#pragma once
struct PolinomData {
    int polynomial_degree;
    double* polynomial_coefficients;
    double variable_X;
    double result;

    PolinomData() {
        polynomial_degree = 0;
        polynomial_coefficients = nullptr;
        variable_X = 0;
        result = 0;
    }


    ~PolinomData() {
        delete[] polynomial_coefficients;
    }

public:
    bool IsCorrect() {
        return polynomial_degree >= 0 && polynomial_coefficients != nullptr;
    }
};